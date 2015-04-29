#ifndef NORLIT_COROUTINE_COROUTINE_H
#define NORLIT_COROUTINE_COROUTINE_H

#include <functional>

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
	static Coroutine current();
  private:
	Data* data;

  private:
	Coroutine(Data*);

  public:
	Coroutine();
	Coroutine(Coroutine&&);
	Coroutine(const Coroutine&);
	Coroutine(std::function<void*(void*)> func);
	~Coroutine();

	void operator =(Coroutine&&);
	void operator =(const Coroutine&);

	Status status();
	void* resume(void* val = nullptr);
	void stop(void* val = nullptr);
};

}
}

#endif