#include "Games.h"

#include <cstdio>

#if TARGET_GAME == SKYSA_3DS

#include "Addresses.h"

#include "3DSAddressUtils.hpp"

#include "alchemystdint.h"

#include "igMetaField.hpp"
#include "igObjectList.hpp"
#include "igHashTable.hpp"





//=============================================================================
// VTable indexes
//=============================================================================
// __internalObjectBase
const avtindex_t Core::__internalObjectBase::kVTIndex_getMeta            = 0;

// igObject, not checked these
const avtindex_t Core::igObject::kVTIndex_isAlikeDeep                    = 1;
const avtindex_t Core::igObject::kVTIndex_userAllocateFields             = 2;
const avtindex_t Core::igObject::kVTIndex_userDeallocateFields           = 3;
const avtindex_t Core::igObject::kVTIndex_userCopyDeep                   = 4;
const avtindex_t Core::igObject::kVTIndex_userCopyShallow                = 5;
const avtindex_t Core::igObject::kVTIndex_userInstantiate                = 6;
const avtindex_t Core::igObject::kVTIndex_userRelease                    = 7;
const avtindex_t Core::igObject::kVTIndex_userResetFields                = 8;
const avtindex_t Core::igObject::kVTIndex_preWrite                       = 9;
const avtindex_t Core::igObject::kVTIndex_postWrite                      = 10;
const avtindex_t Core::igObject::kVTIndex_preFileWrite                   = 11;
const avtindex_t Core::igObject::kVTIndex_postFileWrite                  = 12;
const avtindex_t Core::igObject::kVTIndex_postRead                       = 13;
const avtindex_t Core::igObject::kVTIndex_postFileRead                   = 14;
const avtindex_t Core::igObject::kVTIndex_createDataInheritanceInfo      = 15;
const avtindex_t Core::igObject::kVTIndex_readFromStream                 = 16;
const avtindex_t Core::igObject::kVTIndex_readItemFromStream             = 17;
const avtindex_t Core::igObject::kVTIndex_writeItemToStream              = 18;
const avtindex_t Core::igObject::kVTIndex_clean                          = 19;

// igObjectList
const avtindex_t Core::igObjectListVirtuals::kVTIndex_getElementType     = 0x12;

// igMetaField
const avtindex_t Core::igMetaField::kVTIndex_getGenericMetaField         = 0x13; // Unconfirmed
const avtindex_t Core::igMetaField::kVTIndex_computeSize                 = 0x16; // Unconfirmed
const avtindex_t Core::igMetaField::kVTIndex_computePlatformSize         = 0x17; // Unconfirmed
const avtindex_t Core::igMetaField::kVTIndex_computeRequiredAlignment    = 0x18; // Unconfirmed
const avtindex_t Core::igMetaField::kVTIndex_computePlatformAlignment    = 0x1F; // Unconfirmed
const avtindex_t Core::igMetaField::kVTIndex_getStringFromMemory         = 0x38; // Unconfirmed


// igHashTable
const avtindex_t Core::igHashTableVirtuals::kVTIndex_keyTraitsInvalid    = 0x18; // Unknown
const avtindex_t Core::igHashTableVirtuals::kVTIndex_valueTraitsInvalid  = 0x1B; // Unknown





//=============================================================================
// Function addresses
//=============================================================================
DefineMethod(sprintf,    int,                 __stubs::sprintf,              char*, const char*, ...);
DefineMethod(0x002CD6E8, bool,                Core::igMetaObject_isOfType,   const Core::igMetaObject*, const Core::igMetaObject*);
DefineMethod(0x001A96FC, Core::igMetaObject*, Core::igArkCore_getObjectMeta, Core::igArkCore*, const char*);
DefineMethod(printf,     int,                 _igReportPrintf,               const char*, ...);





//=============================================================================
// Global variables
//=============================================================================
static const asize_t Core_ArkCore                     = 0x0053F1C4;
static const asize_t Core_MetaFieldList               = 0x0053F1B4;
static const asize_t Core_ObjectHandleManager         = 0x005594C8;

Core::igArkCore* __internalGetArkCore()
{
	return (*((Core::igArkCore**)Core_ArkCore));
}

Core::igTVector<Core::igMetaField*>* __internalGetMetaFieldList()
{
	return (*(Core::igTVector<Core::igMetaField*>**)Core_MetaFieldList);
}

Core::igObjectHandleManager* __internalGetObjectHandleManager()
{
	return (*(Core::igObjectHandleManager**)Core_ObjectHandleManager);
}

#endif // TARGET_GAME == SKYSA_3DS
