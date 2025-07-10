#include "Games.h"

#if TARGET_GAME == SKYIM_01_01_00

#include "Addresses.h"

#define PS3_TOC_BASE 0x1C5EBF8
#include "PS3AddressUtils.hpp"

#include "alchemystdint.h"

#include "igMetaField.hpp"
#include "igObjectList.hpp"
#include "igHashTable.hpp"





//=============================================================================
// VTable indexes
//=============================================================================
// __internalObjectBase
const avtindex_t Core::__internalObjectBase::kVTIndex_getMeta            = 0;

// igObject
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
const avtindex_t Core::igObjectListVirtuals::kVTIndex_getElementType     = 0x1C;

// igMetaField
const avtindex_t Core::igMetaField::kVTIndex_getGenericMetaField         = 0x16;
const avtindex_t Core::igMetaField::kVTIndex_computeSize                 = 0x1F;
const avtindex_t Core::igMetaField::kVTIndex_computePlatformSize         = 0x20;
const avtindex_t Core::igMetaField::kVTIndex_computeRequiredAlignment    = 0x23;
const avtindex_t Core::igMetaField::kVTIndex_computePlatformAlignment    = 0x24;
const avtindex_t Core::igMetaField::kVTIndex_getStringFromMemory         = 0x45;
const avtindex_t Core::igMetaField::kVTIndex_getTemplateParameterCount   = 0x52;
const avtindex_t Core::igMetaField::kVTIndex_getTemplateParameter        = 0x53;


// igHashTable
const avtindex_t Core::igHashTableVirtuals::kVTIndex_keyTraitsInvalid    = 0x1B;
const avtindex_t Core::igHashTableVirtuals::kVTIndex_valueTraitsInvalid  = 0x1E;





//=============================================================================
// Function addresses
//=============================================================================
DefineMethod(0x013D0E88, int,                 __stubs::sprintf,              char*, const char*, ...);
DefineMethod(0x00CF7C54, bool,                Core::igMetaObject_isOfType,   const igMetaObject*, const igMetaObject*);
DefineMethod(0x00CECAF8, Core::igMetaObject*, Core::igArkCore_getObjectMeta, igArkCore*, const char*);
DefineMethod(0x00D48084, int,                 _igReportPrintf,               const char*, ...);





//=============================================================================
// Global variables
//=============================================================================
static const asize_t Core_ArkCore                     = 0x01C68F6C;
static const asize_t Core_MetaFieldList               = 0x01C68FF0;

Core::igArkCore* __internalGetArkCore()
{
	return (*((Core::igArkCore**)Core_ArkCore));
}

Core::igTVector<Core::igMetaField*>* __internalGetMetaFieldList()
{
	return (*(Core::igTVector<Core::igMetaField*>**)Core_MetaFieldList);
}

#endif // TARGET_GAME == SKYIM_01_01_00
