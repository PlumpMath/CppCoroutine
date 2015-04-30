#include <windows.h>

#include "common.h"

static const unsigned char context_get_asm[] = {
#include "vs-context_get.inc"
};

static const unsigned char context_set_asm[] = {
#include "vs-context_set.inc"
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
