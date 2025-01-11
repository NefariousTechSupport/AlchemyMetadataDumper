#pragma once

#include <stdint.h>

#include "AddressUtils.h"



//=============================================================================
// C/C++ library stubs, because we mustn't import the standard libraries
//=============================================================================
namespace __stubs
{
	DeclareMethod(int, __sprintf, char* buffer, const char* fmt, ...);
}




//=============================================================================
// Alchemy stuff
//=============================================================================
namespace Core
{
	class igArkCore;
	class igMetaEnum;
	class igMetaField;
	class igMetaObject;

	template<typename T>
	struct igTVector;

	DeclareMethod(bool, igMetaObject_isOfType, const igMetaObject* thisMeta, const igMetaObject* otherMeta);

	DeclareMethod(igMetaObject*, igArkCore_getObjectMeta, igArkCore* thisPtr, const char* name);
}





//=============================================================================
// Alchemy static variables
//=============================================================================

// Do use these
#define ArkCore __internalGetArkCore()
#define ArkCoreMetaFieldList __internalGetMetaFieldList()

// Don't use these
Core::igArkCore* __internalGetArkCore();
Core::igTVector<Core::igMetaField*>* __internalGetMetaFieldList();





//=============================================================================
// General helpers
//=============================================================================

// Define this to be a print method for easier logging or ideally forward it to the game's implementation
DeclareMethod(int, _igReportPrintf, const char* fmt, ...);
