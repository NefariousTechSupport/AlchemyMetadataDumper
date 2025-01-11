#include "Games.h"

#if TARGET_GAME == SKYIM_01_01_00

#include "Addresses.h"

#define PS3_TOC_BASE 0x1C5EBF8
#include "PS3AddressUtils.hpp"


namespace __stubs
{
	DefineMethod(0x013d0e88, int, __sprintf, char* buffer, const char* fmt, ...);
}

namespace Core
{
	DefineMethod(0x00cf7c54, bool, igMetaObject_isOfType, const igMetaObject* thisMeta, const igMetaObject* otherMeta);
	DefineMethod(0x00cecaf8, igMetaObject*, igArkCore_getObjectMeta, igArkCore* thisPtr,  const char* name);
}

DefineMethod(0x00d48084, int, _igReportPrintf, const char* fmt, ...);

Core::igArkCore* __internalGetArkCore()
{
	return (*((Core::igArkCore**)0x01C68F6C));
}

Core::igTVector<Core::igMetaField*>* __internalGetMetaFieldList()
{
	return (*(Core::igTVector<Core::igMetaField*>**)0x01C68FF0);
}

#endif // TARGET_GAME == SKYIM_01_01_00