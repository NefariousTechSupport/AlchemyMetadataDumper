#ifndef ALCHEMY_KAMEK_H
#define ALCHEMY_KAMEK_H

#include "kamekLoader.h"





/*
	Jasleen's top tips for finding the methods

	Use dolphin's debugger to provide symbols for DVDFastOpen & DVDReadPrio

	a call to DVDConvertPathToEntrynum is near the call to DVDFastOpen

	One of the calls to DVDFastOpen is a virtual method of igWiiStorageDevice,
	the next item in the vtable would be the close method, this'll have a call
	to DVDClose.

	_igReportPrintf can be found by searching for the string "%s%p (type %s)\n",
	this'll be used in Core::igObject::printFields, OSReport will be the last
	method called in this.

	Core::igObject::printFields will also call sprintf with the fmt arg "\t%s".

	I don't know how to find OSReport so there's a placeholder implementation.

	Core::igMemoryPool::mallocAligned can be found by looking for the string
	"Core/@jobqueueProfileEventCount", find what references it, the next method
	call is likely the mallocAligned method.

	Core::igMemoryPool::free can be found by looking for a call to
	Core::__internalObjectBase::getClassMetaSafeInternal.

	Core::igGetMemoryPool can also be found in the same method
*/





typedef void*(*igGetMemoryPool_t)(int pool);
typedef void*(*igMemoryPool_mallocAligned_t)(void* thisPtr, u32 size, u32 align);
typedef void (*igMemoryPool_free_t)(void* thisPtr, void* ptr);

struct alchemyLoaderFuncs
{
	loaderFunctions              base;
	igGetMemoryPool_t            igGetMemoryPool;
	igMemoryPool_mallocAligned_t igMemoryPool_mallocAligned;
	igMemoryPool_free_t          igMemoryPool_free;
};





void OSFatal_placeholder(u32* fg, u32* bg, const char* str, ...);





#endif // ALCHEMY_KAMEK_H
