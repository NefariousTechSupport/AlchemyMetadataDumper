#include "AddressUtils.h"

#include "alchemystdint.h"

// This never changes between games on wii
void* GetVirtualFunc(void* vtablePtr, auint32_t index)
{
	return (((void**)vtablePtr)[index + 2]);
}