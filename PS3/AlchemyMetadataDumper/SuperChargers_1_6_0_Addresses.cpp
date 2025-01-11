#include "Games.h"

#if TARGET_GAME == SKYSC_01_06_00

#include "Addresses.h"

#define PS3_TOC_BASE 0x1AE9778
#include "PS3AddressUtils.hpp"


namespace __stubs
{
	DefineMethod(0x0123a23c, int, __sprintf, char* buffer, const char* fmt, ...);
}

namespace Core
{
	DefineMethod(0x00b4c448, bool, igMetaObject_isOfType, const igMetaObject* thisMeta, const igMetaObject* otherMeta);
	DefineMethod(0x00b41130, igMetaObject*, igArkCore_getObjectMeta, igArkCore* thisPtr,  const char* name);
}

DefineMethod(0x00b9b724, int, _igReportPrintf, const char* fmt, ...);

Core::igArkCore* __internalGetArkCore()
{
	return (*((Core::igArkCore**)0x01AEF084));
}

Core::igTVector<Core::igMetaField*>* __internalGetMetaFieldList()
{
	return (*(Core::igTVector<Core::igMetaField*>**)0x01AEF108);
}

#endif // TARGET_GAME == SKYSC_01_06_00