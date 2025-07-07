#pragma once

#include "igDataList.hpp"

#include "Games.h"

namespace Core
{
	class igObjectListVirtuals
	{
	public:
		DefineVirtualFunc_0(igMetaObject*, getElementType);
	};

	template<typename T>
	class igTObjectList : public igTDataList<T*>, public igObjectListVirtuals
	{
	};
	typedef igTObjectList<igObject> igObjectList;
}