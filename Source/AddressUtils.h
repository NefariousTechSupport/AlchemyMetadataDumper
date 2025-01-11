#pragma once

#include <stdint.h>

// Declares a method signature
#define DeclareMethod(ret, name, ...) \
extern ret(*name)(__VA_ARGS__)

// Grabs a virtual method with the given vtable pointer
void* GetVirtualFunc(void* vtablePtr, uint32_t index);