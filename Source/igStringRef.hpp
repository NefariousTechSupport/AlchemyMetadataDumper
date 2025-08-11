#pragma once

#define ReleaseString(string)                                                   \
    do                                                                          \
    {                                                                           \
        if (reinterpret_cast<asize_t>(string) & 0x80000000)                     \
        {                                                                       \
            ::Core::igStringPoolItem::release(string - 0x0C);                   \
        }                                                                       \
    } while(false)

namespace Core
{
	namespace igStringPoolItem
	{
		DeclareMethod(void, release, const char*);
	};

	struct igStringRef
	{
		const char* _value;
		operator const char*() const { return _value; }
		operator const char*() { return _value; }
	};
}