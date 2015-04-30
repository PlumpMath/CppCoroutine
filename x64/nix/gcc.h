void context_setstack(context_t* cor, void* ptr, size_t size) {
    cor->rsp = (uintptr_t)ptr + size - 8;
}

void context_setip(context_t* cor, void(*func)(void)) {
    cor->rip = (uintptr_t)func;
}

__asm(
    ".global context_get;"
    "context_get:"
    ".intel_syntax noprefix;"
    // save return address
    "pop rdx;"
    "mov [rdi + 0x38], rdx;"

    "mov [rdi + 0x00], rbx;"
    // rsp points to address after return
    "mov [rdi + 0x08], rsp;"

    "mov [rdi + 0x10], rbp;"
    "mov [rdi + 0x18], r12;"
    "mov [rdi + 0x20], r13;"
    "mov [rdi + 0x28], r14;"
    "mov [rdi + 0x30], r15;"

    // MXCSR
    "stmxcsr [rdi + 0x40];"
    // X87 Control world
    "fnstcw  [rdi + 0x44];"

    "mov rax, 0;"
    "jmp rdx;"
    ".att_syntax prefix;"
);

__asm(
    ".global context_set;"
    "context_set:"
    ".intel_syntax noprefix;"
    "mov rbx, [rdi + 0x00];"
    "mov rsp, [rdi + 0x08];"
    "mov rbp, [rdi + 0x10];"
    "mov r12, [rdi + 0x18];"
    "mov r13, [rdi + 0x20];"
    "mov r14, [rdi + 0x28];"
    "mov r15, [rdi + 0x30];"

    "ldmxcsr  [rdi + 0x40];"
    "fldcw    [rdi + 0x44];"

    "mov rax, 1;"
    // return address we saved previously
    "jmp [rdi + 0x38];"
    ".att_syntax prefix;"
);
