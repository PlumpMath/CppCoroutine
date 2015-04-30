#include "common.h"

__asm(
    ".global _context_get;"
    ".global context_get;"
    "_context_get:"
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

    // SEH
    "mov eax, fs:[0x0];"
    "mov [ecx + 0x20], eax;"

    // Stack top
    "mov eax, fs:[0x4];"
    "mov [ecx + 0x24], eax;"

    // Stack bottom
    "mov eax, fs:[0x8];"
    "mov [ecx + 0x28], eax;"

    "mov eax, 0;"
    "jmp edx;"
    ".att_syntax prefix;"
);

__asm(
    ".global _context_set;"
    ".global context_set;"
    "_context_set:"
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
    "fldcw  [ecx + 0x1C];"

    // SEH
    "mov eax, [ecx + 0x20];"
    "mov fs:[0x0], eax;"

    // Stack top
    "mov eax, [ecx + 0x24];"
    "mov fs:[0x4], eax;"

    // Stack bottom
    "mov eax, [ecx + 0x28];"
    "mov fs:[0x8], eax;"

    "mov eax, 1;"
    // return address we saved previously
    "jmp [ecx + 0x14];"
    ".att_syntax prefix;"
);

static struct SEHFrame* context_current_seh(void) {
    struct SEHFrame* ret_;
    __asm("fs movl (0), %%eax": "=a"(ret_));
    return ret_;
}
