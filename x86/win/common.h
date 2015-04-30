#include <stdio.h>

/**
 * Windows x86 uses structure exception handling.
 * Detailed information can be accessed by
 * http://www.microsoft.com/msj/0197/exception/exception.aspx
 * http://stackoverflow.com/questions/9249576/seh-setup-for-fibers-with-exception-chain-validation-sehop-active
 * https://github.com/D-Programming-Language/druntime/blob/c39de42dd11311844c0ef90953aa65f333ea55ab/src/core/thread.d
 * Windows Server and Windows 8+ seems all enabled SEHOP, so we need to do SEH searching here
 */

struct SEHFrame {
	struct SEHFrame* prev;
	void* handler;
};

static struct SEHFrame* context_current_seh(void);

static struct SEHFrame* context_last_seh(void) {
	struct SEHFrame* frame = context_current_seh();
	while (frame->prev != (struct SEHFrame*) - 1) {
		frame = frame->prev;
	}
	return frame;
}

static void context_setseh(context_t* ctx) {
	uintptr_t* stack = (uintptr_t*)ctx->stackbase;
	stack[-2] = 0xFFFFFFFF;
	stack[-1] = (uintptr_t)context_last_seh()->handler;
	ctx->seh = (uintptr_t)&stack[-2];
}

void context_setstack(context_t* cor, void* ptr, size_t size) {
	cor->esp = (uintptr_t)ptr + size - 12;
	cor->stackbase = (uintptr_t)ptr + size;
	cor->stacklimit = (uintptr_t)ptr;
	context_setseh(cor);
}

void context_setip(context_t* cor, void(*func)(void)) {
	cor->eip = (uintptr_t)func;
}
