#include "Games.h"

#if TARGET_GAME == SKYTT_01_01_00

#include "Addresses.h"

#define PS3_TOC_BASE 0x927380
#include "PS3AddressUtils.hpp"


namespace __stubs
{
	DefineMethod(0x00496150, int, __sprintf, char* buffer, const char* fmt, ...);
}

namespace Core
{
	DefineMethod(0x001b3864, bool, igMetaObject_isOfType, const igMetaObject* thisMeta, const igMetaObject* otherMeta);
	DefineMethod(0x001a67fc, igMetaObject*, igArkCore_getObjectMeta, igArkCore* thisPtr,  const char* name);
}

DefineMethod(0x001e0b90, int, _igReportPrintf, const char* fmt, ...);

Core::igArkCore* __internalGetArkCore()
{
	return (*((Core::igArkCore**)0x009850fc));
}

Core::igTVector<Core::igMetaField*>* __internalGetMetaFieldList()
{
	return (*(Core::igTVector<Core::igMetaField*>**)0x0098517c);
}

#endif // TARGET_GAME == SKYTT_01_01_00