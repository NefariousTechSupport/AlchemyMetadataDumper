#include "Games.h"

#if TARGET_GAME == SKYSA_01_00_03

#include "Addresses.h"

#include "WiiAddressUtils.hpp"

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
const avtindex_t Core::igMetaField::kVTIndex_getGenericMetaField         = 0x13; // Confirmed
const avtindex_t Core::igMetaField::kVTIndex_computeSize                 = 0x16; // Confirmed
const avtindex_t Core::igMetaField::kVTIndex_computePlatformSize         = 0x17; // Maybe??
const avtindex_t Core::igMetaField::kVTIndex_computeRequiredAlignment    = 0x18; // Likely
const avtindex_t Core::igMetaField::kVTIndex_computePlatformAlignment    = 0x1F; // Unknown
const avtindex_t Core::igMetaField::kVTIndex_getStringFromMemory         = 0x38; // Confirmed


// igHashTable
const avtindex_t Core::igHashTableVirtuals::kVTIndex_keyTraitsInvalid    = 0x18; // Unknown
const avtindex_t Core::igHashTableVirtuals::kVTIndex_valueTraitsInvalid  = 0x1B; // Unknown





//=============================================================================
// Function addresses
//=============================================================================
DefineMethod(0x803571CC, int,                 __stubs::sprintf,              char*, const char*, ...);
DefineMethod(0x800E398C, bool,                Core::igMetaObject_isOfType,   const Core::igMetaObject*, const Core::igMetaObject*);
DefineMethod(0x800D8F9C, Core::igMetaObject*, Core::igArkCore_getObjectMeta, Core::igArkCore*, const char*);
DefineMethod(0x80112DB4, int,                 _igReportPrintf,               const char*, ...);





//=============================================================================
// Global variables
//=============================================================================
static const asize_t Core_ArkCore                     = 0x8066aa94;
static const asize_t Core_MetaFieldList               = 0x0098517c;

Core::igArkCore* __internalGetArkCore()
{
	return (*((Core::igArkCore**)Core_ArkCore));
}

Core::igTVector<Core::igMetaField*>* __internalGetMetaFieldList()
{
	return (*(Core::igTVector<Core::igMetaField*>**)Core_MetaFieldList);
}

#endif // TARGET_GAME == SKYSA_01_00_03
