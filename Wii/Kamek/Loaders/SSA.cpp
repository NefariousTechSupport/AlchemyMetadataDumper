#include "AlchemyKamek.h"

int loadIntoSSA();

#define ALCHEMY_KAMEK_DEFAULT_POOL 6

kmCondWritePointer(0x804CF8D0, 0x00000000, loadIntoSSA); // EU rev3, adds to the end of the initialisers array

void* allocAdapter(u32 size, bool isForCode, const loaderFunctions* funcs)
{
	const alchemyLoaderFuncs* alchemyFuncs = reinterpret_cast<const alchemyLoaderFuncs*>(funcs);
	void* memoryPool = alchemyFuncs->igGetMemoryPool(ALCHEMY_KAMEK_DEFAULT_POOL);
	return alchemyFuncs->igMemoryPool_mallocAligned(memoryPool, size, 0x100);
}

void freeAdapter(void* buffer, bool isForCode, const loaderFunctions *funcs)
{
	const alchemyLoaderFuncs* alchemyFuncs = reinterpret_cast<const alchemyLoaderFuncs*>(funcs);
	void* memoryPool = alchemyFuncs->igGetMemoryPool(ALCHEMY_KAMEK_DEFAULT_POOL);
	alchemyFuncs->igMemoryPool_free(memoryPool, buffer);
}


const alchemyLoaderFuncs functions_p = {
	{
		reinterpret_cast<OSReport_t>(0x8000b398),
		reinterpret_cast<OSFatal_t>(OSFatal_placeholder), // Unknown
		reinterpret_cast<DVDConvertPathToEntrynum_t>(0x803659c0),
		reinterpret_cast<DVDFastOpen_t>(0x80365cd0),
		reinterpret_cast<DVDReadPrio_t>(0x803660a0),
		reinterpret_cast<DVDClose_t>(0x80365e60),
		reinterpret_cast<sprintf_t>(0x803571cc),
		allocAdapter,
		freeAdapter,
	},
	reinterpret_cast<igGetMemoryPool_t>(0x80103f68),
	reinterpret_cast<igMemoryPool_mallocAligned_t>(0x801031c8),
	reinterpret_cast<igMemoryPool_free_t>(0x800dca08)
};

int loadIntoSSA()
{
	char region = 'P';
	int version = 3;
	const alchemyLoaderFuncs* funcs = &functions_p;

	char path[64];
	if (version == 0)
	{
		funcs->base.sprintf(path, "/Code/%c.bin", region);
	}
	else
	{
		funcs->base.sprintf(path, "/Code/%c%d.bin", region, version);
	}

	loadKamekBinaryFromDisc(&funcs->base, path);

	return 1;
}

