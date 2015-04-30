#include "common.h"

__asm(
    ".global context_get;"
    "context_get:"
    ".intel_syntax noprefix;"
    // save return address
    "pop rdx;"
    "mov [rcx + 0x48], rdx;"

    "mov [rcx + 0x00], rbx;"
    // rsp points to address after return
    "mov [rcx + 0x08], rsp;"

    "mov [rcx + 0x10], rbp;"
    "mov [rcx + 0x18], rsi;"
    "mov [rcx + 0x20], rdi;"
    "mov [rcx + 0x28], r12;"
    "mov [rcx + 0x30], r13;"
    "mov [rcx + 0x38], r14;"
    "mov [rcx + 0x40], r15;"

    "movdqu [rcx + 0x50], xmm6;"
    "movdqu [rcx + 0x60], xmm7;"
    "movdqu [rcx + 0x70], xmm8;"
    "movdqu [rcx + 0x80], xmm9;"
    "movdqu [rcx + 0x90], xmm10;"
    "movdqu [rcx + 0xA0], xmm11;"
    "movdqu [rcx + 0xB0], xmm12;"
    "movdqu [rcx + 0xC0], xmm13;"
    "movdqu [rcx + 0xD0], xmm14;"
    "movdqu [rcx + 0xE0], xmm15;"

    // stack top
    "mov rax, gs:[0x8];"
    "mov [rcx + 0xF0], rax;"

    // stack bottom
    "mov rax, gs:[0x10];"
    "mov [rcx + 0xF8], rax;"

    "stmxcsr [rcx + 0x100];"
    "fnstcw  [rcx + 0x104];"

    "mov rax, 0;"
    "jmp rdx;"
    ".att_syntax prefix;"
);

__asm(
    ".global context_set;"
    "context_set:"
    ".intel_syntax noprefix;"
    "mov rbx, [rcx + 0x00];"
    "mov rsp, [rcx + 0x08];"
    "mov rbp, [rcx + 0x10];"
    "mov rsi, [rcx + 0x18];"
    "mov rdi, [rcx + 0x20];"
    "mov r12, [rcx + 0x28];"
    "mov r13, [rcx + 0x30];"
    "mov r14, [rcx + 0x38];"
    "mov r15, [rcx + 0x40];"

    "movdqu xmm6 , [rcx + 0x50];"
    "movdqu xmm7 , [rcx + 0x60];"
    "movdqu xmm8 , [rcx + 0x70];"
    "movdqu xmm9 , [rcx + 0x80];"
    "movdqu xmm10, [rcx + 0x90];"
    "movdqu xmm11, [rcx + 0xA0];"
    "movdqu xmm12, [rcx + 0xB0];"
    "movdqu xmm13, [rcx + 0xC0];"
    "movdqu xmm14, [rcx + 0xD0];"
    "movdqu xmm15, [rcx + 0xE0];"

    "mov  r10, gs:[0x30];"

    // stack top
    "mov rax, [rcx + 0xF0];"
    "mov [r10+0x8], rax;"

    // stack bottom
    "mov rax, [rcx + 0xF8];"
    "mov [r10+0x10], rax;"

    "ldmxcsr [rcx + 0x100];"
    "fldcw   [rcx + 0x104];"

    "mov rax, 1;"
    // return address we saved previously
    "jmp [rcx + 0x48];"
    ".att_syntax prefix;"
);
