#pragma once

#include "igDataList.hpp"

#include "Games.h"

namespace Core
{
	template<typename T>
	class igTObjectList : public igTDataList<T*>
	{
	public:
#if TARGET_GAME >= SKYSC_01_00_00 && TARGET_GAME <= SKYIM_01_01_00
		const uint32_t kVTIndexGetElementType = 0x1C;
#elif TARGET_GAME >= SKYTT_01_00_00 && TARGET_GAME <= SKYTT_01_01_00
		const uint32_t kVTIndexGetElementType = 0x18;
#else
#error "Unimplemented game"
#endif

		DefineVirtualFunc_0(kVTIndexGetElementType, igMetaObject*, getElementType);
	};
	typedef igTObjectList<igObject> igObjectList;
}