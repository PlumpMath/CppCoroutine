#include "context.h"

#ifdef __cplusplus
namespace norlit {
namespace coroutine {
extern "C" {
#endif

#include NORLIT_MACRO_STIRNGIFY(NORLIT_ARCH_NAME/NORLIT_OS_NAME/NORLIT_COMPILER_NAME.h)

void context_swap(context_t* store, const context_t* target) {
	if (!context_get(store)) {
		context_set(target);
	}
}

#ifdef __cplusplus
}
}
}
#endif
