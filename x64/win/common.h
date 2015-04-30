void context_setstack(context_t* cor, void* ptr, size_t size) {
	cor->rsp = (uintptr_t)ptr + size - 8;
	cor->stack_top = (uintptr_t)ptr + size;
	cor->stack_bottom = (uintptr_t)ptr;
}

void context_setip(context_t* cor, void(*func)(void)) {
	cor->rip = (uintptr_t)func;
}