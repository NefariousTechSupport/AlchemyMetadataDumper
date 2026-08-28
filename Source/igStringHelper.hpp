#pragma once

#include "alchemystdint.h"

#include "AddressUtils.h"

#if TARGET_CAFE
// Cafe does not have getElementType in the igTObjectList vtables so we have to make a substring without "List"
// and look the type of that, which is why we need this

namespace Core
{
	namespace igStringHelper
    {
        DeclareMethod(char*, copyn,   const char*, char*, int);
        DeclareMethod(int,   length,  const char*);
        DeclareMethod(int,   compare, const char*, const char*);
    }
}


#endif // TARGET_CAFE
