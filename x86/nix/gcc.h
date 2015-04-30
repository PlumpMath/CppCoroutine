void context_setstack(context_t* cor, void* ptr, size_t size) {
    cor->esp = (uintptr_t)ptr + size - 4;
}

void context_setip(context_t* cor, void(*func)(void)) {
    cor->eip = (uintptr_t)func;
}

__asm(
    ".global context_get;"
    "context_get:"
    ".intel_syntax noprefix;"
    // save return address
    "pop edx;"
    "mov ecx, [esp];"
    "mov [ecx + 0x14], edx;"

    "mov [ecx + 0x00], ebx;"
    // rsp points to address after return
    "mov [ecx + 0x04], esp;"

    "mov [ecx + 0x08], ebp;"
    "mov [ecx + 0x0C], esi;"
    "mov [ecx + 0x10], edi;"

    "stmxcsr [ecx + 0x18];"
    "fnstcw  [ecx + 0x1C];"

    "mov eax, 0;"
    "jmp edx;"
    ".att_syntax prefix;"
);

__asm(
    ".global context_set;"
    "context_set:"
    ".intel_syntax noprefix;"
    // It's okay to just pop here, because we will override esp later
    "mov ecx, [esp + 4];"
    "mov ebx, [ecx + 0x00];"
    "mov esp, [ecx + 0x04];"
    "mov ebp, [ecx + 0x08];"
    "mov esi, [ecx + 0x0C];"
    "mov edi, [ecx + 0x10];"

    "ldmxcsr [ecx + 0x18];"
    "fldcw   [ecx + 0x1C];"

    "mov eax, 1;"
    // return address we saved previously
    "jmp [ecx + 0x14];"
    ".att_syntax prefix;"
);
