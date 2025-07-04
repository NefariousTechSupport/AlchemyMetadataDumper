#pragma once

#include "igBaseMeta.hpp"
#include "igDataList.hpp"
#include "igObjectList.hpp"

namespace Core
{
	class igMetaEnum : public igBaseMeta
	{
	public:
		bool _flags;
		igTDataList<const char*>* _names;
		igTDataList<aint32_t>* _values;
		igObjectList* _attributes;
		igObjectList* _valueAttributes;
	};
}