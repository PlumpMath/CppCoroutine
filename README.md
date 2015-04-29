C++Coroutine
===
This is a extremely lightweight coroutine library for C++ 
Consist of only 4 file (1 .c, 1 .cc and 2 .h files)

Supported platform and compiler are linux/windows compiled via gcc/clang/visual c++ targeted on x86 or x64 devices.

A example of using this (implementation of async/await and promise) can be found at https://gist.github.com/nbdd0121/0c739ca139d58ae12290

API Reference
---
The main class is norlit::coroutine::Coroutine.
The norlit::coroutine::context_t and series of functions are available if you want to use low-level ucontext-like functions.

| Member Function | Signature                                   | Description
|-----------------|---------------------------------------------|-------------
| (constructor)   | Coroutine()                                 | Initialize a empty instance that refers to no coroutine
| (constructor)   | Coroutine(Coroutine&&)                      | Move constructor; original Coroutine will refer to no coroutine
| (constructor)   | Coroutine(const Coroutine&)                 | Copy constructor; two Coroutine instances refer to the same coroutine
| (constructor)   | Coroutine(std::function&lt;void\*(void\*)>) | Create a new oroutine with given function
| (destructor)    | ~Coroutine()                                | Delete the coroutine if it is pointed by none of the Coroutine instance
| operator =      | void(Coroutine&&)                           | Reassign the current Coroutine instance
| operator =      | void(const Coroutine&)                      | Reassign the current Coroutine instance
| empty           | bool()                                      | Test whether a coroutine is retained by the current Coroutine instance
| stop            | void\*(void\* = nullptr)                    | Force the coroutine to stop. not really recommended to use.
| resume          | void\*(void\* = nullptr)                    | Resume/Start the coroutine
| status          | Coroutine::Status()                         | Get the status of coroutine

| Static Function | Signature                                   | Description
|-----------------|---------------------------------------------|-------------
| yield           | void\*(void\* = nullptr)                    | Yield the current coroutine
| current         | Coroutine()                                 | Get the running coroutine

| Enumeration     | Description
|-----------------|-------------
| READY           | Coroutine is not yet started
| RUNNING         | Coroutine is running
| SUSPENDED       | yield() is called by coroutine and resume is not called after that
| STOPPED         | stop() is called or the coroutine returns
