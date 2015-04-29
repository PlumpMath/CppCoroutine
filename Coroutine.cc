#include "Coroutine.h"
#include <stdexcept>
#include <cstdlib>

namespace norlit {
namespace coroutine {

Coroutine* Coroutine::current_ = nullptr;

void* Coroutine::yield(void* ret) {
	Coroutine* cur = current_;
	current_ = nullptr;
	switch (cur->status) {
		case RUNNING:
			cur->status = SUSPENDED;
			cur->ret = ret;
			context_swap(&cur->context, &cur->caller);
			return cur->ret;
		default:
			throw std::logic_error{"Unexpected status of coroutine"};
	}
}

Coroutine& Coroutine::current() {
	return *current_;
}

Coroutine::Coroutine(std::function<void*(void*)> func): func(func) {
	stack = malloc(65536);
	context_get(&context);
	context_setstack(&context, stack, 65536);
	context_setip(&context, []() {
		current_->stop(current_->func(current_->ret));
	});
}

Coroutine::~Coroutine() {
	free(stack);
}

void* Coroutine::resume(void* val) {
	Coroutine* currentBackup = current_;
	current_ = this;
	switch (status) {
		case READY:
		case SUSPENDED:
			status = RUNNING;
			ret = val;
			context_swap(&caller, &context);
			current_ = currentBackup;
			break;
		default:
			throw std::logic_error{"Unexpected status of coroutine"};
	}
	return ret;
}

void Coroutine::stop(void* val) {
	switch (status) {
		case RUNNING:
			status = STOPPED;
			ret = val;
			context_swap(&context, &caller);
			break;
		case READY:
		case SUSPENDED:
			status = STOPPED;
			break;
		default:
			throw std::logic_error{"Unexpected status of coroutine"};
	}
}

}
}
