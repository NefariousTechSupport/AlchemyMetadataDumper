#pragma once

#include "igBaseMeta.hpp"
#include "igDataList.hpp"
#include "igObjectList.hpp"

namespace Core
{
	class igMetaEnum
#if TARGET_GAME >= SKYST_BEGIN
	: public igBaseMeta
#else // TARGET_GAME >= SKYST_BEGIN
	: public igObject
#endif // TARGET_GAME >= SKYST_BEGIN
	{
	public:
		bool _flags;
#if TARGET_GAME < SKYTT_BEGIN
		const char* _name;
#endif // TARGET_GAME < SKYSA_END
		igTDataList<const char*>* _names;
		igTDataList<aint32_t>* _values;
		igObjectList* _attributes;
		igObjectList* _valueAttributes;
	};
}