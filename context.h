#ifndef NORLIT_COROUTINE_CONTEXT_H
#define NORLIT_COROUTINE_CONTEXT_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
namespace norlit {
namespace coroutine {
extern "C" {
#endif

#define NORLIT_OS_WIN 0
#define NORLIT_OS_NIX 1

#define NORLIT_ARCH_X86 0
#define NORLIT_ARCH_X64 1

#define NORLIT_COMPILER_GCC	0
#define NORLIT_COMPILER_VS	1

#define NORLIT_ABI_CDECL 0
#define NORLIT_ABI_WIN64 1
#define NORLIT_ABI_SYSV  2


#if defined(_WIN32)
#	define NORLIT_OS NORLIT_OS_WIN
#	if defined(_WIN64)
#		define NORLIT_ARCH NORLIT_ARCH_X64
#	else
#		define NORLIT_ARCH NORLIT_ARCH_X86
#	endif
#	if defined(__MINGW32__)
#		define NORLIT_COMPILER NORLIT_COMPILER_GCC
#	else
#		define NORLIT_COMPILER NORLIT_COMPILER_VS
#	endif
#elif defined(__CYGWIN__)
#	define NORLIT_OS NORLIT_OS_WIN
#	if defined(__CYGWIN32__)
#		define NORLIT_ARCH NORLIT_ARCH_X86
#	else
#		define NORLIT_ARCH NORLIT_ARCH_X64
#	endif
#	define NORLIT_COMPILER NORLIT_COMPILER_GCC
#else
#	define NORLIT_OS NORLIT_OS_NIX
#	if defined(__x86_64__)
#		define NORLIT_ARCH NORLIT_ARCH_X64
#	else
#		define NORLIT_ARCH NORLIT_ARCH_X86
#	endif
#	define NORLIT_COMPILER NORLIT_COMPILER_GCC
#endif

#if NORLIT_OS == NORLIT_OS_WIN
#	define NORLIT_OS_NAME win
#elif NORLIT_OS == NORLIT_OS_NIX
#	define NORLIT_OS_NAME nix
#else
#	error Unknown OS
#endif

#if NORLIT_ARCH == NORLIT_ARCH_X86
#	define NORLIT_ARCH_NAME x86
#elif NORLIT_ARCH == NORLIT_ARCH_X64
#	define NORLIT_ARCH_NAME x64
#else
#	error Unknown OS
#endif

#if NORLIT_COMPILER == NORLIT_COMPILER_GCC
#	define NORLIT_COMPILER_NAME gcc
#elif NORLIT_COMPILER == NORLIT_COMPILER_VS
#	define NORLIT_COMPILER_NAME vs
#else
#	error Unknown OS
#endif

#define NORLIT_MACRO_STIRNGIFY_N(arg) #arg
#define NORLIT_MACRO_STIRNGIFY(arg) NORLIT_MACRO_STIRNGIFY_N(arg)

#include NORLIT_MACRO_STIRNGIFY(NORLIT_ARCH_NAME/NORLIT_OS_NAME/context.h)

int  context_get(context_t*);
void context_set(const context_t*);

void context_swap(context_t* store, const context_t* target);
void context_setstack(context_t* cor, void* ptr, size_t size);
void context_setip(context_t* cor, void(*func)(void));

#ifdef __cplusplus
}
}
}
#endif

#endif