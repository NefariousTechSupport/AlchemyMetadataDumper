#include "AddressUtils.h"

#include "alchemystdint.h"

// This never changes between games on wii
void* GetVirtualFunc(void* vtablePtr, auint32_t index)
{
	return (((void**)vtablePtr)[index + 2]);
}

const char* FixupStaticString(const char* stringPtr)
{
	if (reinterpret_cast<asize_t>(stringPtr) < 0x80000000)
	{
		stringPtr = reinterpret_cast<const char*>(reinterpret_cast<asize_t>(stringPtr) | 0x80000000);
	}

	return stringPtr;
}