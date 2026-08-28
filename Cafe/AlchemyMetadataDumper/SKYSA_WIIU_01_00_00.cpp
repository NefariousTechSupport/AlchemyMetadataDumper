#include "Games.h"

#if TARGET_GAME == SKYSA_WIIU_01_00_00

#include "Addresses.h"

#include "CafeAddressUtils.hpp"

#include "alchemystdint.h"

#include "igMetaField.hpp"
#include "igObjectList.hpp"
#include "igHashTable.hpp"

#include "igStringHelper.hpp"

// SSA Wii U is missing some of the virtual functions listed here but a lot 
// seems unused anyway so I'm using this to not break the project
#define DUMMY_FAKE_VTABLE_INDEX (const avtindex_t) 9999


//=============================================================================
// VTable indexes
//=============================================================================
// __internalObjectBase
const avtindex_t Core::__internalObjectBase::kVTIndex_getMeta            = 0;

// igObject
const avtindex_t Core::igObject::kVTIndex_isAlikeDeep                    = DUMMY_FAKE_VTABLE_INDEX;
const avtindex_t Core::igObject::kVTIndex_userAllocateFields             = 1;
const avtindex_t Core::igObject::kVTIndex_userDeallocateFields           = 2;
const avtindex_t Core::igObject::kVTIndex_userCopyDeep                   = 3;
const avtindex_t Core::igObject::kVTIndex_userCopyShallow                = 4;
const avtindex_t Core::igObject::kVTIndex_userInstantiate                = 5;
const avtindex_t Core::igObject::kVTIndex_userRelease                    = 6;
const avtindex_t Core::igObject::kVTIndex_userResetFields                = 7;
const avtindex_t Core::igObject::kVTIndex_preWrite                       = DUMMY_FAKE_VTABLE_INDEX;
const avtindex_t Core::igObject::kVTIndex_postWrite                      = DUMMY_FAKE_VTABLE_INDEX;
const avtindex_t Core::igObject::kVTIndex_preFileWrite                   = DUMMY_FAKE_VTABLE_INDEX;
const avtindex_t Core::igObject::kVTIndex_postFileWrite                  = DUMMY_FAKE_VTABLE_INDEX;
const avtindex_t Core::igObject::kVTIndex_postRead                       = 12;
const avtindex_t Core::igObject::kVTIndex_postFileRead                   = 13;
const avtindex_t Core::igObject::kVTIndex_createDataInheritanceInfo      = DUMMY_FAKE_VTABLE_INDEX; // never seen this in any of the executables I checked
const avtindex_t Core::igObject::kVTIndex_readFromStream                 = DUMMY_FAKE_VTABLE_INDEX; // same here
const avtindex_t Core::igObject::kVTIndex_readItemFromStream             = DUMMY_FAKE_VTABLE_INDEX; // same here
const avtindex_t Core::igObject::kVTIndex_writeItemToStream              = DUMMY_FAKE_VTABLE_INDEX; // same here
const avtindex_t Core::igObject::kVTIndex_clean                          = 14;

// igObjectList
const avtindex_t Core::igObjectListVirtuals::kVTIndex_getElementType     = DUMMY_FAKE_VTABLE_INDEX;

// igMetaField
const avtindex_t Core::igMetaField::kVTIndex_getGenericMetaField         = 0x11;
const avtindex_t Core::igMetaField::kVTIndex_computeSize                 = 0x1A;
const avtindex_t Core::igMetaField::kVTIndex_computePlatformSize         = 0x1B;
const avtindex_t Core::igMetaField::kVTIndex_computeRequiredAlignment    = 0x1E;
const avtindex_t Core::igMetaField::kVTIndex_computePlatformAlignment    = 0x1F;
const avtindex_t Core::igMetaField::kVTIndex_getStringFromMemory         = 0x40;




//=============================================================================
// Function addresses
//=============================================================================
DefineMethod(0x0257d690, int,                 __stubs::sprintf,              char*, const char*, ...);
DefineMethod(0x0215bb10, bool,                Core::igMetaObject_isOfType,   const igMetaObject*, const igMetaObject*);
DefineMethod(0x02154454, Core::igMetaObject*, Core::igArkCore_getObjectMeta, igArkCore*, const char*);
DefineMethod(0x0218a9fc, int,                 _igReportPrintf,               const char*, ...);

// Cafe only thing
DefineMethod(0x021a6318, char*,               Core::igStringHelper::copyn,   const char*, char*, int);
DefineMethod(0x021a5848, int,                 Core::igStringHelper::length,  const char*);
DefineMethod(0x021a5454, int,                 Core::igStringHelper::compare, const char*, const char*);

//=============================================================================
// Global variables
//=============================================================================
static const asize_t Core_ArkCore                     = 0x100cd25c;
static const asize_t Core_MetaFieldList               = 0x100cd248;
static const asize_t Core_ObjectHandleManager         = 0x100cd590;

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
#endif // TARGET_GAME == SKYSA_WIIU_01_00_00
