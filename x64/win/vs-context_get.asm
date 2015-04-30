[bits 64]

context_get:
    ; save return address
    pop rdx
    mov [rcx + 048h], rdx

    mov [rcx + 000h], rbx
    ; rsp points to address after return
    mov [rcx + 008h], rsp;

    mov [rcx + 010h], rbp
    mov [rcx + 018h], rsi
    mov [rcx + 020h], rdi
    mov [rcx + 028h], r12
    mov [rcx + 030h], r13
    mov [rcx + 038h], r14
    mov [rcx + 040h], r15

    movdqu [rcx + 050h], xmm6
    movdqu [rcx + 060h], xmm7
    movdqu [rcx + 070h], xmm8
    movdqu [rcx + 080h], xmm9
    movdqu [rcx + 090h], xmm10
    movdqu [rcx + 0A0h], xmm11
    movdqu [rcx + 0B0h], xmm12
    movdqu [rcx + 0C0h], xmm13
    movdqu [rcx + 0D0h], xmm14
    movdqu [rcx + 0E0h], xmm15

    ; stack top
    gs mov rax, [08h]
    mov [rcx + 0F0h], rax

    ; stack bottom
    gs mov rax, [10h]
    mov [rcx + 0F8h], rax

    stmxcsr [rcx + 100h]
    fnstcw  [rcx + 104h]

    mov rax, 0
    jmp rdx
