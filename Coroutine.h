#ifndef NORLIT_COROUTINE_COROUTINE_H
#define NORLIT_COROUTINE_COROUTINE_H

#include <functional>
#include <exception>

#include "context.h"

namespace norlit {
namespace coroutine {

class Coroutine {
  public:
	enum Status {
		READY,
		RUNNING,
		SUSPENDED,
		STOPPED
	};
  private:
	struct Data;
  private:
	static Data* current_;

  public:
	static void* yield(void* ret = nullptr);
	static void* yield_throw(std::exception_ptr ex);
	static Coroutine current();

	template<typename T>
	static void* yield_throw(T t) {
		return yield_throw(std::make_exception_ptr(std::move(t)));
	}
  private:
	Data* data;

  private:
	Coroutine(Data*);

  public:
	Coroutine();
	Coroutine(Coroutine&&) throw();
	Coroutine(const Coroutine&);
	Coroutine(std::function<void*(void*)> func);
	~Coroutine();

	void operator =(Coroutine&&) throw();
	void operator =(const Coroutine&);

	bool empty();
	void stop(void* val = nullptr);
	void* resume(void* val = nullptr);
	void* throw_exception(std::exception_ptr ex);
	Status status();

	template<typename T>
	void* throw_exception(T t) {
		return throw_exception(std::make_exception_ptr(std::move(t)));
	}
};

}
}

#endif