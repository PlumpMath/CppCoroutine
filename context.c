#include "context.h"

#if NORLIT_COMPILER == NORLIT_COMPILER_GCC
#	if NORLIT_ABI_USED == NORLIT_ABI_CDECL

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

    "mov eax, 1;"
    // return address we saved previously
    "jmp [ecx + 0x14];"
    ".att_syntax prefix;"
);

#	elif NORLIT_ABI_USED == NORLIT_ABI_WIN64

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

    "stmxcsr [rcx + 0xF0];"

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

    "ldmxcsr [rcx + 0xF0];"

    "mov rax, 1;"
    // return address we saved previously
    "jmp [rcx + 0x48];"
    ".att_syntax prefix;"
);

#	elif NORLIT_ABI_USED == NORLIT_ABI_SYSV

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

    "mov rax, 1;"
    // return address we saved previously
    "jmp [rdi + 0x38];"
    ".att_syntax prefix;"
);

#	else
#		error Unsupported ABI
#	endif
#elif NORLIT_COMPILER == NORLIT_COMPILER_VS
#	if NORLIT_ABI_USED == NORLIT_ABI_CDECL

__declspec(naked) int context_get(context_t* ctx) {
	__asm {
		pop edx
		mov ecx, [esp]
		mov [ecx + 0x14], edx

		mov [ecx + 0x00], ebx
		// rsp points to address after return
		mov [ecx + 0x04], esp

		mov [ecx + 0x08], ebp
		mov [ecx + 0x0C], esi
		mov [ecx + 0x10], edi

		stmxcsr [ecx + 0x18]

		mov eax, 0
		jmp edx
	}
}

__declspec(naked) void context_set(const context_t* ctx) {
	__asm{
		// It's okay to just pop here, because we will override esp later
		mov ecx, [esp+4]
		mov ebx, [ecx + 0x00]
		mov esp, [ecx + 0x04]
		mov ebp, [ecx + 0x08]
		mov esi, [ecx + 0x0C]
		mov edi, [ecx + 0x10]

		ldmxcsr  [ecx + 0x18]

		mov eax, 1
		// return address we saved previously
		mov ecx, [ecx + 0x14]
		jmp ecx
	}
}

#	elif NORLIT_ABI_USED == NORLIT_ABI_WIN64

#		include <windows.h>

static const unsigned char context_get_asm[] = {
	0x5a,
	0x48, 0x89, 0x51, 0x48,
	0x48, 0x89, 0x19,
	0x48, 0x89, 0x61, 0x08,
	0x48, 0x89, 0x69, 0x10,
	0x48, 0x89, 0x71, 0x18,
	0x48, 0x89, 0x79, 0x20,
	0x4c, 0x89, 0x61, 0x28,
	0x4c, 0x89, 0x69, 0x30,
	0x4c, 0x89, 0x71, 0x38,
	0x4c, 0x89, 0x79, 0x40,
	0xf3, 0x0f, 0x7f, 0x71, 0x50,
	0xf3, 0x0f, 0x7f, 0x79, 0x60,
	0xf3, 0x44, 0x0f, 0x7f, 0x41, 0x70,
	0xf3, 0x44, 0x0f, 0x7f, 0x89, 0x80, 0x00, 0x00, 0x00,
	0xf3, 0x44, 0x0f, 0x7f, 0x91, 0x90, 0x00, 0x00, 0x00,
	0xf3, 0x44, 0x0f, 0x7f, 0x99, 0xa0, 0x00, 0x00, 0x00,
	0xf3, 0x44, 0x0f, 0x7f, 0xa1, 0xb0, 0x00, 0x00, 0x00,
	0xf3, 0x44, 0x0f, 0x7f, 0xa9, 0xc0, 0x00, 0x00, 0x00,
	0xf3, 0x44, 0x0f, 0x7f, 0xb1, 0xd0, 0x00, 0x00, 0x00,
	0xf3, 0x44, 0x0f, 0x7f, 0xb9, 0xe0, 0x00, 0x00, 0x00,
	0x0f, 0xae, 0x99, 0xf0, 0x00, 0x00, 0x00,
	0xb8, 0x00, 0x00, 0x00, 0x00,
	0xff, 0xe2,
};

static const unsigned char context_set_asm[] = {
	0x48, 0x8b, 0x19,
	0x48, 0x8b, 0x61, 0x08,
	0x48, 0x8b, 0x69, 0x10,
	0x48, 0x8b, 0x71, 0x18,
	0x48, 0x8b, 0x79, 0x20,
	0x4c, 0x8b, 0x61, 0x28,
	0x4c, 0x8b, 0x69, 0x30,
	0x4c, 0x8b, 0x71, 0x38,
	0x4c, 0x8b, 0x79, 0x40,
	0xf3, 0x0f, 0x6f, 0x71, 0x50,
	0xf3, 0x0f, 0x6f, 0x79, 0x60,
	0xf3, 0x44, 0x0f, 0x6f, 0x41, 0x70,
	0xf3, 0x44, 0x0f, 0x6f, 0x89, 0x80, 0x00, 0x00, 0x00,
	0xf3, 0x44, 0x0f, 0x6f, 0x91, 0x90, 0x00, 0x00, 0x00,
	0xf3, 0x44, 0x0f, 0x6f, 0x99, 0xa0, 0x00, 0x00, 0x00,
	0xf3, 0x44, 0x0f, 0x6f, 0xa1, 0xb0, 0x00, 0x00, 0x00,
	0xf3, 0x44, 0x0f, 0x6f, 0xa9, 0xc0, 0x00, 0x00, 0x00,
	0xf3, 0x44, 0x0f, 0x6f, 0xb1, 0xd0, 0x00, 0x00, 0x00,
	0xf3, 0x44, 0x0f, 0x6f, 0xb9, 0xe0, 0x00, 0x00, 0x00,
	0x0f, 0xae, 0x91, 0xf0, 0x00, 0x00, 0x00,
	0xb8, 0x01, 0x00, 0x00, 0x00,
	0xff, 0x61, 0x48,
};

static int(*context_get_exec)(context_t*) = NULL;
static void(*context_set_exec)(const context_t*) = NULL;

int context_get(context_t* ctx) {
	if (!context_get_exec) {
		context_get_exec = VirtualAlloc(NULL, sizeof(context_get_asm), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		memcpy(context_get_exec, context_get_asm, sizeof(context_get_asm));
	}
	return context_get_exec(ctx);
}

void context_set(const context_t* ctx) {
	if (!context_set_exec) {
		context_set_exec = VirtualAlloc(NULL, sizeof(context_set_asm), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		memcpy(context_set_exec, context_set_asm, sizeof(context_set_asm));
	}
	context_set_exec(ctx);
}

#	else
#		error Unsupported ABI
#	endif

#else
#	error Unsupported Compiler
#endif

#if NORLIT_ABI_USED == NORLIT_ABI_CDECL

void context_setstack(context_t* cor, void* ptr, size_t size) {
	cor->esp = (uintptr_t)ptr + size;
}

void context_setip(context_t* cor, void(*func)(void)) {
	cor->eip = (uintptr_t)func;
}

#elif NORLIT_ABI_USED == NORLIT_ABI_WIN64 || NORLIT_ABI_USED == NORLIT_ABI_SYSV

void context_setstack(context_t* cor, void* ptr, size_t size) {
	cor->rsp = (uintptr_t)ptr + size - 8;
}

void context_setip(context_t* cor, void(*func)(void)) {
	cor->rip = (uintptr_t)func;
}

#else
#	error Unsupported ABI
#endif

void context_swap(context_t* store, const context_t* target) {
	if (!context_get(store)) {
		context_set(target);
	}
}
