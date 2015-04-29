#ifndef NORLIT_COROUTINE_CONTEXT_H
#define NORLIT_COROUTINE_CONTEXT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

#define NORLIT_ABI_CDECL 0
#define NORLIT_ABI_WIN64 1
#define NORLIT_ABI_SYSV  2
#define NORLIT_COMPILER_GCC	0
#define NORLIT_COMPILER_VS	1

#if defined(_WIN32) || defined (__CYGWIN__)
#	if defined(_WIN64) || (defined(__CYGWIN__) && !defined(__CYGWIN32__))
#		define NORLIT_ABI_USED NORLIT_ABI_WIN64
#	else
#		define NORLIT_ABI_USED NORLIT_ABI_CDECL
#	endif
#	if defined(__CYGWIN__) || defined(__MINGW32__)
#		define NORLIT_COMPILER NORLIT_COMPILER_GCC
#	else
#		define NORLIT_COMPILER NORLIT_COMPILER_VS
#	endif
#else
#	if defined(__x86_64__)
#		define NORLIT_ABI_USED NORLIT_ABI_SYSV
#	else
#		define NORLIT_ABI_USED NORLIT_ABI_CDECL
#	endif
#	define NORLIT_COMPILER NORLIT_COMPILER_GCC
#endif

#if NORLIT_ABI_USED == NORLIT_ABI_CDECL
typedef struct {
	uint32_t ebx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;
	uint32_t eip;
	uint32_t mxcsr;
} context_t;
#elif NORLIT_ABI_USED == NORLIT_ABI_WIN64
typedef struct {
	uint64_t rbx;
	uint64_t rsp;
	uint64_t rbp;
	uint64_t rsi;
	uint64_t rdi;
	uint64_t r12;
	uint64_t r13;
	uint64_t r14;
	uint64_t r15;
	uint64_t rip;
	uint64_t xmm6[2];
	uint64_t xmm7[2];
	uint64_t xmm8[2];
	uint64_t xmm9[2];
	uint64_t xmm10[2];
	uint64_t xmm11[2];
	uint64_t xmm12[2];
	uint64_t xmm13[2];
	uint64_t xmm14[2];
	uint64_t xmm15[2];
	uint32_t mxcsr;
} context_t;
#elif NORLIT_ABI_USED == NORLIT_ABI_SYSV
typedef struct {
	uint64_t rbx;
	uint64_t rsp;
	uint64_t rbp;
	uint64_t r12;
	uint64_t r13;
	uint64_t r14;
	uint64_t r15;
	uint64_t rip;
	uint32_t mxcsr;
} context_t;
#else
#	error Unsupported ABI
#endif

int  context_get(context_t*);
void context_set(const context_t*);

void context_swap(context_t* store, const context_t* target);
void context_setstack(context_t* cor, void* ptr, size_t size);
void context_setip(context_t* cor, void(*func)(void));

#ifdef __cplusplus
}
#endif

#endif