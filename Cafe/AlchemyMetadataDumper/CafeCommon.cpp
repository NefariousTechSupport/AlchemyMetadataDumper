#include "AddressUtils.h"

#include "alchemystdint.h"

void* GetVirtualFunc(void* vtablePtr, auint32_t index)
{
	return (((void**)vtablePtr)[3 + index * 2]);
}

