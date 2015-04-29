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
	static Coroutine* current_;

  public:
	static void* yield(void* ret = nullptr);
	static Coroutine& current();
  private:
	context_t caller;
	context_t context;
	std::function<void*(void*)> func;
	Status status = READY;
	void* ret;
	void* stack;

  public:
	Coroutine(std::function<void*(void*)> func);
	~Coroutine();
	void* resume(void* val = nullptr);
	void stop(void* val = nullptr);
};

}
}

