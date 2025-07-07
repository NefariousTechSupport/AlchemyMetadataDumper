#pragma once

#include "igContainer.hpp"

#include "igMemory.hpp"
#include "Games.h"

namespace Core
{
	class igHashTableVirtuals : public igContainer
	{
	public:
		DefineVirtualFunc_0(void*, keyTraitsInvalid);
		DefineVirtualFunc_0(void*, valueTraitsInvalid);
	};

	template<typename T, typename U>
	class igTUHashTable : public igHashTableVirtuals
	{
	public:
		igMemory<T> _values;
		igMemory<U> _keys;
		int _hashItemCount;
		bool _autoRehash;
		float _loadFactor;
	};
}