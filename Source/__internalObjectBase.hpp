#pragma once

#include "alchemystdint.h"

#include "cpp11hacks.hpp"

#include "AddressUtils.h"

#define CallVirtualFunc_0(index, ret) ((ret(*)(void*))GetVirtualFunc(reinterpret_cast<const __internalObjectBase*>(this)->_vTable, index))(this)
#define CallVirtualFunc_0_c(index, ret) ((ret(*)(const void*))GetVirtualFunc(reinterpret_cast<const __internalObjectBase*>(this)->_vTable, index))(this)
#define CallVirtualFunc_1(index, ret, type0, value0) ((ret(*)(void*, type0))GetVirtualFunc(reinterpret_cast<const __internalObjectBase*>(this)->_vTable, index))(this, value0)
#define CallVirtualFunc_2(index, ret, type0, value0, type1, value1) ((ret(*)(void*, type0, type1))GetVirtualFunc(reinterpret_cast<const __internalObjectBase*>(this)->_vTable, index))(this, value0, value1)

#define DeclareVirtualFunc(name)                                               \
	static const auint32_t kVTIndex_##name

#define DefineVirtualFunc_0(ret, name)                                         \
	DeclareVirtualFunc(name);                                                  \
	inline ret name()                                                          \
	{                                                                          \
	    const auint32_t index = kVTIndex_##name;                               \
	    return CallVirtualFunc_0(index, ret);                                  \
	}

#define DefineVirtualFunc_0_c(ret, name)                                       \
	DeclareVirtualFunc(name);                                                  \
	inline ret name() const                                                    \
	{                                                                          \
	    const auint32_t index = kVTIndex_##name;                               \
	    return CallVirtualFunc_0_c(index, ret);                                \
	}

#define DefineVirtualFunc_1(ret, name, type0, value0)                          \
	DeclareVirtualFunc(name);                                                  \
	inline ret name(type0 value0)                                              \
	{                                                                          \
	    const auint32_t index = kVTIndex_##name;                               \
	    return CallVirtualFunc_1(index, ret, type0, value0);                   \
	}

#define DefineVirtualFunc_2(ret, name, type0, value0, type1, value1)           \
	DeclareVirtualFunc(name);                                                  \
	inline ret name(type0 value0, type1 value1)                                \
	{                                                                          \
	    const auint32_t index = kVTIndex_##name;                               \
	    return CallVirtualFunc_2(index, ret, type0, value0, type1, value1);    \
	}

#define DefineVirtualFunc_3(ret, name, type0, value0, type1, value1, type2, value2) \
	DeclareVirtualFunc(name);                                                              \
	inline ret name(type0 value0, type1 value1, type2 value2)                              \
	{                                                                                      \
	    const auint32_t index = kVTIndex_##name;                                           \
	    return CallVirtualFunc_2(index, ret, type0, value0, type1, value1, type2, value2); \
	}

namespace Core
{
	class igMetaObject;

	class __internalObjectBase
	{
	public:
		void** _vTable;
		auint32_t _referenceCount;

		DefineVirtualFunc_0_c(igMetaObject*, getMeta)
	};
}

//static_assert(offsetof(Core::__internalObjectBase, _vTable) == 0x00, "vtable doesn't start at 0??");