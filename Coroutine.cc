#include "Coroutine.h"
#include <stdexcept>
#include <cstdlib>

namespace norlit {
namespace coroutine {

Coroutine::Data* Coroutine::current_ = nullptr;

struct Coroutine::Data {
	context_t caller;
	context_t context;
	std::function<void*(void*)> func;
	Status status_ = READY;
	void* ret = nullptr;
	void* stack = nullptr;
	int refCount = 0;

	Data(std::function<void*(void*)> func) : func(func) {
		stack = malloc(65536);
		context_get(&context);
		context_setstack(&context, stack, 65536);
		context_setip(&context, [] () {
			current_->stop(current_->func(current_->ret));
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

	void* yield(void* ret) {
		current_ = nullptr;
		switch (status_) {
			case RUNNING:
				status_ = SUSPENDED;
				ret = ret;
				context_swap(&context, &caller);
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

			decreaseReferenceCount();
			return val;
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

Coroutine Coroutine::current() {
	return current_;
}

Coroutine::Coroutine(Data* data) : data(data) {
	data->increaseReferenceCount();
}

Coroutine::Coroutine() : data{nullptr} {

}

Coroutine::Coroutine(Coroutine&& c) : data{ c.data } {
	c.data = nullptr;
}

Coroutine::Coroutine(const Coroutine& c) : Coroutine{ c.data } {

}

Coroutine::Coroutine(std::function<void*(void*)> func) : Coroutine{ new Data{ func } } {

}

Coroutine::~Coroutine() {
	if (data) {
		data->decreaseReferenceCount();
	}
}

void Coroutine::operator=(Coroutine&& c) {
	data = c.data;
	c.data = nullptr;
}

void Coroutine::operator=(const Coroutine& c) {
	this->~Coroutine();
	data = c.data;
	data->increaseReferenceCount();
}

Coroutine::Status Coroutine::status() {
	return data->status_;
}

void* Coroutine::resume(void* val) {
	if (!data) {
		throw std::runtime_error{"Invoke resume on null"};
	}
	return data->resume(val);
}

void Coroutine::stop(void* val) {
	if (!data) {
		throw std::runtime_error{ "Invoke resume on null" };
	}
	data->stop(val);
}

}
}
