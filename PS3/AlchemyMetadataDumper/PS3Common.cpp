#include "AddressUtils.h"

#include "alchemystdint.h"

// This never changes between games on ps3
void* GetVirtualFunc(void* vtablePtr, auint32_t index)
{
	return (((void**)vtablePtr)[index]);
}