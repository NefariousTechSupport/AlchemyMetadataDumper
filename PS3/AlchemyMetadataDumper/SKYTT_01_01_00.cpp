#include "Games.h"

#if TARGET_GAME == SKYTT_01_01_00

#include "Addresses.h"

#define PS3_TOC_BASE 0x927380
#include "PS3AddressUtils.hpp"

#include "alchemystdint.h"

#include "igMetaField.hpp"
#include "igObjectList.hpp"
#include "igHashTable.hpp"





//=============================================================================
// VTable indexes
//=============================================================================
// __internalObjectBase
const auint32_t Core::__internalObjectBase::kVTIndex_getMeta            = 0;

// igObject
const auint32_t Core::igObject::kVTIndex_isAlikeDeep                    = 1;
const auint32_t Core::igObject::kVTIndex_userAllocateFields             = 2;
const auint32_t Core::igObject::kVTIndex_userDeallocateFields           = 3;
const auint32_t Core::igObject::kVTIndex_userCopyDeep                   = 4;
const auint32_t Core::igObject::kVTIndex_userCopyShallow                = 5;
const auint32_t Core::igObject::kVTIndex_userInstantiate                = 6;
const auint32_t Core::igObject::kVTIndex_userRelease                    = 7;
const auint32_t Core::igObject::kVTIndex_userResetFields                = 8;
const auint32_t Core::igObject::kVTIndex_preWrite                       = 9;
const auint32_t Core::igObject::kVTIndex_postWrite                      = 10;
const auint32_t Core::igObject::kVTIndex_preFileWrite                   = 11;
const auint32_t Core::igObject::kVTIndex_postFileWrite                  = 12;
const auint32_t Core::igObject::kVTIndex_postRead                       = 13;
const auint32_t Core::igObject::kVTIndex_postFileRead                   = 14;
const auint32_t Core::igObject::kVTIndex_createDataInheritanceInfo      = 15;
const auint32_t Core::igObject::kVTIndex_readFromStream                 = 16;
const auint32_t Core::igObject::kVTIndex_readItemFromStream             = 17;
const auint32_t Core::igObject::kVTIndex_writeItemToStream              = 18;
const auint32_t Core::igObject::kVTIndex_clean                          = 19;

// igObjectList
const auint32_t Core::igObjectListVirtuals::kVTIndex_getElementType     = 0x18;

// igMetaField
const auint32_t Core::igMetaField::kVTIndex_getGenericMetaField         = 0x11;
const auint32_t Core::igMetaField::kVTIndex_computeSize                 = 0x1A;
const auint32_t Core::igMetaField::kVTIndex_computePlatformSize         = 0x1B;
const auint32_t Core::igMetaField::kVTIndex_computeRequiredAlignment    = 0x1E;
const auint32_t Core::igMetaField::kVTIndex_computePlatformAlignment    = 0x1F;
const auint32_t Core::igMetaField::kVTIndex_getStringFromMemory         = 0x3E;
const auint32_t Core::igMetaField::kVTIndex_getTemplateParameterCount   = 0x4B;
const auint32_t Core::igMetaField::kVTIndex_getTemplateParameter        = 0x4C;


// igHashTable
const auint32_t Core::igHashTableVirtuals::kVTIndex_keyTraitsInvalid    = 0x18;
const auint32_t Core::igHashTableVirtuals::kVTIndex_valueTraitsInvalid  = 0x1B;





//=============================================================================
// Function addresses
//=============================================================================
DefineMethod(0x00496150, int,                 __stubs::sprintf,              char*, const char*, ...);
DefineMethod(0x001B3864, bool,                Core::igMetaObject_isOfType,   const igMetaObject*, const igMetaObject*);
DefineMethod(0x001A67FC, Core::igMetaObject*, Core::igArkCore_getObjectMeta, igArkCore*, const char*);
DefineMethod(0x001E0B90, int,                 _igReportPrintf,               const char*, ...);





//=============================================================================
// Global variables
//=============================================================================
static const asize_t Core_ArkCore                     = 0x009850fc;
static const asize_t Core_MetaFieldList               = 0x0098517c;

Core::igArkCore* __internalGetArkCore()
{
	return (*((Core::igArkCore**)Core_ArkCore));
}

Core::igTVector<Core::igMetaField*>* __internalGetMetaFieldList()
{
	return (*(Core::igTVector<Core::igMetaField*>**)Core_MetaFieldList);
}

#endif // TARGET_GAME == SKYTT_01_01_00
