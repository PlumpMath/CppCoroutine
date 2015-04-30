#include "Coroutine.h"
#include <stdexcept>
#include <exception>
#include <cstdlib>
#include <cstdio>

namespace norlit {
namespace coroutine {

Coroutine::Data* Coroutine::current_ = nullptr;

struct Coroutine::Data {
	context_t caller;
	context_t context;
	std::function<void*(void*)> func;
	std::exception_ptr exception;
	Status status_ = READY;
	void* ret = nullptr;
	void* stack = nullptr;
	int refCount = 0;

	Data(std::function<void*(void*)>&& func) : func{ std::move(func) } {
		stack = malloc(65536);
		context_get(&context);
		context_setstack(&context, stack, 65536);
		context_setip(&context, [] () {
			try {
				current_->stop(current_->func(current_->ret));
			} catch (...) {
				current_->exception = std::current_exception();
				current_->stop(nullptr);
			}
		});
	}

	~Data() {
		free(stack);
	}

	void stop(void* val) {
		switch (status_) {
			case RUNNING:
				status_ = STOPPED;
				ret = val;
				/* Since we are not going back to this coroutine, we do not use swap */
				context_set(&caller);
				break;
			/* Just mark as stopped, so coroutine cannot be launched again */
			case READY:
			case SUSPENDED:
				status_ = STOPPED;
				break;
			default:
				throw std::logic_error{ "Unexpected status of coroutine" };
		}
	}

	void increaseReferenceCount() {
		refCount++;
	}

	void decreaseReferenceCount() {
		if (--refCount == 0) {
			delete this;
		}
	}

	void* yield(void* val) {
		current_ = nullptr;
		switch (status_) {
			case RUNNING:
				status_ = SUSPENDED;
				ret = val;
				context_swap(&context, &caller);
				if (exception) {
					std::exception_ptr ex = exception;
					exception = nullptr;
					std::rethrow_exception(ex);
				}
				return ret;
			default:
				throw std::logic_error{ "Unexpected status of coroutine" };
		}
	}

	void* resume(void* val);
};

void* Coroutine::Data::resume(void* val) {
	switch (status_) {
		case READY:
		case SUSPENDED: {
			Data* currentBackup = current_;
			current_ = this;

			/* Since we sets current_, we need to make sure that we actually */
			increaseReferenceCount();

			status_ = RUNNING;
			ret = val;
			context_swap(&caller, &context);
			current_ = currentBackup;
			val = ret;

			if (exception) {
				std::exception_ptr ex = exception;
				exception = nullptr;
				decreaseReferenceCount();
				std::rethrow_exception(ex);
			} else {
				val = ret;
				decreaseReferenceCount();
				return val;
			}
		}
		default:
			throw std::logic_error{ "Unexpected status of coroutine" };
	}
}

void* Coroutine::yield(void* ret) {
	if (!current_) {
		throw std::runtime_error{ "No coroutine is currently running" };
	}
	return current_->yield(ret);
}

void* Coroutine::yield_throw(std::exception_ptr ex) {
	if (!current_) {
		throw std::runtime_error{ "No coroutine is currently running" };
	}
	current_->exception = ex;
	return current_->yield(nullptr);
}

Coroutine Coroutine::current() {
	return current_;
}

Coroutine::Coroutine(Data* data) : data(data) {
	if (data)
		data->increaseReferenceCount();
}

Coroutine::Coroutine() : data{ nullptr } {

}

Coroutine::Coroutine(Coroutine&& c) throw() : data {c.data} {
	c.data = nullptr;
}

Coroutine::Coroutine(const Coroutine& c) : Coroutine{ c.data } {

}

Coroutine::Coroutine(std::function<void*(void*)> func) : Coroutine{ new Data{ std::move(func) } } {

}

Coroutine::~Coroutine() {
	if (data) {
		data->decreaseReferenceCount();
	}
}

void Coroutine::operator=(Coroutine&& c) throw() {
	data = c.data;
	c.data = nullptr;
}

void Coroutine::operator=(const Coroutine& c) {
	this->~Coroutine();
	data = c.data;
	if (data)
		data->increaseReferenceCount();
}

bool Coroutine::empty() {
	return data == nullptr;
}

Coroutine::Status Coroutine::status() {
	return data->status_;
}

void* Coroutine::resume(void* val) {
	if (!data) {
		throw std::runtime_error{ "Invoke resume on null" };
	}
	return data->resume(val);
}

void* Coroutine::throw_exception(std::exception_ptr ex) {
	if (!data) {
		throw std::runtime_error{ "Invoke throwException on null" };
	}
	if (data->status_ != SUSPENDED) {
		throw std::logic_error{ "Unexpected status of coroutine" };
	}
	data->exception = ex;
	return data->resume(nullptr);
}

void Coroutine::stop(void* val) {
	if (!data) {
		throw std::runtime_error{ "Invoke resume on null" };
	}
	data->stop(val);
}

}
}
