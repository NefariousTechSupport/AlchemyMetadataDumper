#include "PS3AddressUtils.hpp"


// This never changes between games on ps3
void* GetVirtualFunc(void* vtablePtr, uint32_t index)
{
	return (((void**)vtablePtr)[index]);
}