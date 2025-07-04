#pragma once

#include "igContainer.hpp"

#include "igMemory.hpp"
#include "Games.h"

namespace Core
{
	template<typename T, typename U>
	class igTUHashTable : igContainer
	{
	public:
#if TARGET_GAME >= SKYSC_01_00_00 && SKYIM_01_01_00 <= SKYIM_01_01_00
		const auint32_t kVTIndexKeyTraitsInvalid   = 0x1B;
		const auint32_t kVTIndexValueTraitsInvalid = 0x1E;
#elif TARGET_GAME >= SKYTT_01_00_00 && TARGET_GAME <= SKYTT_01_01_00
		const auint32_t kVTIndexKeyTraitsInvalid   = 0x18;
		const auint32_t kVTIndexValueTraitsInvalid = 0x1B;
#else
#error "Unimplemented game"
#endif 
		
		igMemory<T> _values;
		igMemory<U> _keys;
		int _hashItemCount;
		bool _autoRehash;
		float _loadFactor;
	};
}