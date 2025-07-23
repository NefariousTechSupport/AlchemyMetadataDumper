#pragma once

#include "igBaseMeta.hpp"
#include "igDataList.hpp"
#include "igObjectList.hpp"

namespace Core
{
	class igMetaEnum
#if TARGET_GAME >= SKYTT_BEGIN
	: public igBaseMeta
#else // TARGET_GAME >= SKYTT_BEGIN
	: public igObject
#endif // TARGET_GAME >= SKYTT_BEGIN
	{
	public:
		bool _flags;
#if TARGET_GAME < SKYTT_BEGIN
	private:
		const char* _name;
	public:
#endif // TARGET_GAME < SKYSA_END
		igTDataList<const char*>* _names;
		igTDataList<aint32_t>* _values;
		igObjectList* _attributes;
#if TARGET_GAME >= SKYTT_BEGIN
		igObjectList* _valueAttributes;
#endif // TARGET_GAME >= SKYTT_BEGIN

		inline const char* getName()
		{
			asize_t name = reinterpret_cast<asize_t>(_name);
#if TARGET_WII // They use the most significant bit to store something on wii
			name = name | 0x80000000;
#endif // TARGET_WII
			return reinterpret_cast<const char*>(name);
		}
	};
}