#pragma once

#include <stdint.h>

#include "cpp11hacks.hpp"

#include "AddressUtils.h"

#define CallVirtualFunc_0(index, ret) ((ret(*)(__internalObjectBase*))GetVirtualFunc(_vTable, index))(this)
#define CallVirtualFunc_0_c(index, ret) ((ret(*)(const __internalObjectBase*))GetVirtualFunc(_vTable, index))(this)
#define CallVirtualFunc_1(index, ret, type0, value0) ((ret(*)(__internalObjectBase*, type0))GetVirtualFunc(_vTable, index))(this, value0)
#define CallVirtualFunc_2(index, ret, type0, value0, type1, value1) ((ret(*)(__internalObjectBase*, type0, type1))GetVirtualFunc(_vTable, index))(this, value0, value1)

#define DefineVirtualFunc_0(index, ret, name)   inline ret name()       { return CallVirtualFunc_0(index, ret); }
#define DefineVirtualFunc_0_c(index, ret, name) inline ret name() const { return CallVirtualFunc_0_c(index, ret); }
#define DefineVirtualFunc_1(index, ret, name, type0, value0) inline ret name(type0 value0) { return CallVirtualFunc_1(index, ret, type0, value0); }
#define DefineVirtualFunc_2(index, ret, name, type0, value0, type1, value1) inline ret name(type0 value0, type1 value1) { return CallVirtualFunc_2(index, ret, type0, value0, type1, value1); }
#define DefineVirtualFunc_3(index, ret, name, type0, value0, type1, value1, type2, value2) inline ret name(type0 value0, type1 value1, type2 value2) { return CallVirtualFunc_2(index, ret, type0, value0, type1, value1, type2, value2); }

namespace Core
{
	class igMetaObject;

	class __internalObjectBase
	{
	public:
		void** _vTable;
		uint32_t _referenceCount;

		DefineVirtualFunc_0_c(0, igMetaObject*, getMeta)
	};
}

//static_assert(offsetof(Core::__internalObjectBase, _vTable) == 0x00, "vtable doesn't start at 0??");