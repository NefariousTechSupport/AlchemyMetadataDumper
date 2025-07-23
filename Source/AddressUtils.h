#pragma once

#include "alchemystdint.h"

// Declares a method signature
#define DeclareMethod(ret, name, ...) \
extern ret(*name)(__VA_ARGS__)

// Grabs a virtual method with the given vtable pointer
void* GetVirtualFunc(void* vtablePtr, auint32_t index);

#if TARGET_WII
// Fixes a wii static string
const char* FixupStaticString(const char* stringPtr);

#define FIX_STRING(stringPtr) FixupStaticString(stringPtr)
#else
#define FIX_STRING(stringPtr) (stringPtr)
#endif // TARGET_WII