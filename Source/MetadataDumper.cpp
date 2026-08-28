#include "MetadataDumper.hpp"

#include "Addresses.h"

#include "igArkCore.hpp"
#include "igObjectList.hpp"
#include "igObjectHandleManager.hpp"
#include "igHashTable.hpp"
#include "igHandle.hpp"
#include "igMetaEnum.hpp"
#include "igMetaObject.hpp"
#include "igMetaField.hpp"
#include "igStringHelper.hpp"

#include "InterfaceResolver.hpp"
#include "tfbScriptObject.hpp"

#include "cpp11hacks.hpp"
#include "asleep.h"
#include "fileio.hpp"

#define DEBUG_LOGS 0

#if DEBUG_LOGS
#define DEBUGPRINTF(fmt, ...) _igReportPrintf("[DEBUGPRINTF] " fmt, __VA_ARGS__)
#else
#define DEBUGPRINTF(fmt, ...)
#endif // DEBUG_LOGS


static Core::igMetaField defaultMetaField = Core::igMetaField();

bool streq(const char* a, const char* b)
{
	if (!a || !b)
	{
		return a == b;
	}

	while(*a)
	{
		if (*a != *b)
		{
			return false;
		}

		b++;
		a++;
	}

	return *b == 0x00;
}

static Core::igMetaEnum* platformMetaEnum = 0;

struct
{
	Core::igMetaObject* dynamicMetaEnumType;
	FileWriter* writer;
} metaenumContext;

void SetupMetaenumDumping(FileWriter& writer)
{
	metaenumContext.dynamicMetaEnumType = Core::igArkCore_getObjectMeta(ArkCore, "igDotNetDynamicMetaEnum");

	metaenumContext.writer = &writer;

	metaenumContext.writer->WriteText(12, "<metaenums>\n");
}

void DumpMetaEnum(Core::igMetaEnum* metaEnum)
{
	char buf[512];
	int len;

	// Flag that we've read this one already so that it doesn't get dumped multiple times
	auint8_t& metaenumFlags = *reinterpret_cast<auint8_t*>(&metaEnum->_flags);
	if (metaenumFlags & 0x80)
	{
		return;
	}
	metaenumFlags |= 0x80;

	if (!platformMetaEnum && streq(metaEnum->getName(), "IG_CORE_PLATFORM"))
	{
		platformMetaEnum = metaEnum;
	}
	
	WriteFormattedText(PTR(metaenumContext.writer), "\t<metaenum refname=\"%s\">\n", metaEnum->getName());
	for(int j = 0; j < metaEnum->_names->_count; j++)
	{
		WriteFormattedText(PTR(metaenumContext.writer), "\t\t<value name=\"%s\" value=\"%d\"/>\n", FIX_STRING(metaEnum->_names->get(j)), metaEnum->_values->get(j));
	}
	WriteFormattedText(PTR(metaenumContext.writer), "\t</metaenum>\n", metaEnum->getName());
}

void StopMetaenumDumping()
{
	metaenumContext.writer->WriteText(12, "</metaenums>");
}

void DumpMetaEnums(FileWriter& writer)
{
	Core::igMemory<Core::igMetaEnum*>& metaEnums = ArkCore->_metaEnumHashTable->_values;
	for(int i = 0; i < metaEnums._size / sizeof(Core::igMetaEnum*); i++)
	{
		Core::igMetaEnum* metaEnum = metaEnums.get(i);
		if(metaEnum == nullptr)
		{
			continue;
		}

		if(metaenumContext.dynamicMetaEnumType && metaEnum->getMeta()->isOfType(metaenumContext.dynamicMetaEnumType))
		{
			continue;
		}

		DumpMetaEnum(metaEnum);
	}
}

void DumpMetaFieldList()
{
	char buf[512];
	int len;

	FileWriter writer = FileWriter("metafields.xml");
	writer.WriteText(13, "<metafields>\n");

	for (int t = 0; t < ArkCoreMetaFieldList->_count; t++)
	{
		Core::igMetaField* field = ArkCoreMetaFieldList->get(t);
		WriteFormattedTextIndented(REF(writer), 1, "<metafield name=\"%s\">\n", field->getMeta()->getName());

		for (int i = 0; i < platformMetaEnum->_names->_count; i++)
		{
			const char* platformName = FIX_STRING(platformMetaEnum->_names->get(i));

			if (streq(platformName, "IG_CORE_PLATFORM_DEFAULT")
			 || streq(platformName, "IG_CORE_PLATFORM_DEPRECATED")
			 || streq(platformName, "IG_CORE_PLATFORM_MAX"))
			{
				continue;
			}

			WriteFormattedTextIndented(REF(writer), 2, "<platforminfo platform=\"%s\" align=\"0x%02X\" size=\"0x%02X\"/>\n", platformName, field->computePlatformAlignment(i), field->computePlatformSize(i));
		}

		WriteIndentedText(REF(writer), 1, 13, "</metafield>\n");
	}

	writer.WriteText(14, "</metafields>\n");
}

void DumpMetaField(FileWriter& writer, int indent, Core::igMetaField* metafield, bool root = false)
{
	char buf[512];
	int len;
	if (metafield == nullptr)
	{
		return;
	}

	const Core::igMetaObject* fieldType = metafield->getMeta();

	// temporarily removing static as a workaround since static constructors don't work on CemuPatchCompiler, TODO find a way to make these static again
	const Core::igMetaObject* metaFieldMetaObject    = Core::igArkCore_getObjectMeta(ArkCore, "igMetaField");
	const Core::igMetaObject* memRefMetaObject       = Core::igArkCore_getObjectMeta(ArkCore, "igMemoryRefMetaField");
	const Core::igMetaObject* memRefHandleMetaObject = Core::igArkCore_getObjectMeta(ArkCore, "igMemoryRefHandleMetaField");
	const Core::igMetaObject* refMetaObject          = Core::igArkCore_getObjectMeta(ArkCore, "igRefMetaField");
	const Core::igMetaObject* objectRefMetaObject    = Core::igArkCore_getObjectMeta(ArkCore, "igObjectRefMetaField");
	const Core::igMetaObject* handleMetaObject       = Core::igArkCore_getObjectMeta(ArkCore, "igHandleMetaField");
	const Core::igMetaObject* bitFieldMetaObject     = Core::igArkCore_getObjectMeta(ArkCore, "igBitFieldMetaField");
	const Core::igMetaObject* enumMetaObject         = Core::igArkCore_getObjectMeta(ArkCore, "igEnumMetaField");
	const Core::igMetaObject* staticMetaObject       = Core::igArkCore_getObjectMeta(ArkCore, "igStaticMetaField");
	const Core::igMetaObject* stringMetaObject       = Core::igArkCore_getObjectMeta(ArkCore, "igStringMetaField");
	const Core::igMetaObject* propertyMetaObject     = Core::igArkCore_getObjectMeta(ArkCore, "igPropertyFieldMetaField");
	const Core::igMetaObject* ucharMetaObject        = Core::igArkCore_getObjectMeta(ArkCore, "igUnsignedCharMetaField");
	const Core::igMetaObject* structMetaObject       = Core::igArkCore_getObjectMeta(ArkCore, "igStructMetaField");
	const Core::igMetaObject* vectorMetaObject       = Core::igArkCore_getObjectMeta(ArkCore, "igVectorMetaField");

	//_igReportPrintf("memref is null? %d \n", memRefMetaObject == nullptr ? 1 : 0);

	bool hasChildNodes = false;


	WriteFormattedTextIndented(REF(writer),
	                           indent,
	                           "<metafield type=\"%s\""
	                           " offset=\"0x%04X\"",
	                           metafield->getMeta()->getName(),
	                           metafield->_offset
	                           );

	if (root && metafield->getName())
	{
		WriteFormattedText(REF(writer), " name=\"%s\"", metafield->getName());
	}

	if (metafield->_properties._copyMethod != 3)
	{
		WriteFormattedText(REF(writer), " copyMethod=\"%s\"", Core::fieldProperties::sPropertyTypeStrings[metafield->_properties._copyMethod]);
	}
	if (metafield->_properties._resetMethod != 3)
	{
		WriteFormattedText(REF(writer), " resetMethod=\"%s\"", Core::fieldProperties::sPropertyTypeStrings[metafield->_properties._resetMethod]);
	}
	if (metafield->_properties._isAlikeMethod != 3)
	{
		WriteFormattedText(REF(writer), " isAlikeMethod=\"%s\"", Core::fieldProperties::sPropertyTypeStrings[metafield->_properties._isAlikeMethod]);
	}
	if (metafield->_properties._itemsCopyMethod != 3)
	{
		WriteFormattedText(REF(writer), " itemsCopyMethod=\"%s\"", Core::fieldProperties::sPropertyTypeStrings[metafield->_properties._itemsCopyMethod]);
	}
	if (metafield->_properties._keysCopyMethod != 3)
	{
		WriteFormattedText(REF(writer), " keysCopyMethod=\"%s\"", Core::fieldProperties::sPropertyTypeStrings[metafield->_properties._keysCopyMethod]);
	}
	if (!metafield->_properties._persistent)
	{
		writer.WriteText(19, " persistent=\"false\"");
	}
	if (metafield->_properties._hasInvariance)
	{
		writer.WriteText(21, " hasInvariance=\"true\"");
	}
	if (metafield->_properties._hasPoolName)
	{
		writer.WriteText(19, " hasPoolName=\"true\"");
	}
	if (metafield->_properties._mutable)
	{
		writer.WriteText(15, " mutable=\"true\"");
	}
	if (!metafield->_properties._implicitAlignment)
	{
		writer.WriteText(26, " implicitAlignment=\"false\"");
		WriteFormattedText(REF(writer), " requiredAlignment=\"%d\"", metafield->computeRequiredAlignment());
	}


	// Do not if/else this, only if
	// e.g., igRefMetaField can be a base class for several of the ones below

	if (fieldType->isOfType(refMetaObject))
	{
		WriteFormattedText(REF(writer), " construct=\"%s\" destruct=\"%s\" reconstruct=\"%s\" refCounted=\"%s\"",
			((Core::igRefMetaField*)metafield)->_construct   ? "true" : "false",
			((Core::igRefMetaField*)metafield)->_destruct    ? "true" : "false",
			((Core::igRefMetaField*)metafield)->_reconstruct ? "true" : "false",
			((Core::igRefMetaField*)metafield)->_refCounted  ? "true" : "false"
			);
	}

	if (fieldType->isOfType(objectRefMetaObject))
	{
		WriteFormattedText(REF(writer), " metaobject=\"%s\"", 
			((Core::igObjectRefMetaField*)metafield)->_metaObject ? ((Core::igObjectRefMetaField*)metafield)->_metaObject->getName() : "igObject"
			);
	}

	if (fieldType->isOfType(handleMetaObject))
	{
		WriteFormattedText(REF(writer), " metaobject=\"%s\"", 
			((Core::igHandleMetaField*)metafield)->_metaObject ? ((Core::igHandleMetaField*)metafield)->_metaObject->getName() : "igObject"
			);
	}

	if (fieldType->isOfType(memRefMetaObject) || fieldType->isOfType(memRefHandleMetaObject))
	{
		writer.WriteText(13, " memType=\"f0\"");
		hasChildNodes = true;
	}

	if (fieldType->isOfType(bitFieldMetaObject))
	{
		WriteFormattedText(REF(writer),
		                   " shift=\"0x%02X\" bits=\"0x%02X\" storageField=\"%s\" assignmentField=\"f0\"",
		                   ((Core::igBitFieldMetaField*)metafield)->_shift,
		                   ((Core::igBitFieldMetaField*)metafield)->_bits,
		                   ((Core::igBitFieldMetaField*)metafield)->_storageMetaField->getName()
		                   );
		hasChildNodes = true;
	}

	if (fieldType->isOfType(enumMetaObject))
	{
		Core::igMetaEnum*(*getMetaEnumFunc)() = ((Core::igEnumMetaField*)metafield)->_getMetaEnumFunction;
		WriteFormattedText(REF(writer),
		                   " metaenum=\"%s\"",
		                   getMetaEnumFunc ? getMetaEnumFunc()->getName() : "(null)"
		                   );

		if (getMetaEnumFunc)
		{
			DumpMetaEnum(getMetaEnumFunc());
		}
	}

	if (fieldType->isOfType(staticMetaObject))
	{
		writer.WriteText(22, " storageMetaField=\"f0\"");
		hasChildNodes = true;
	}

	if (fieldType->isOfType(propertyMetaObject))
	{
		writer.WriteText(20, " innerMetaField=\"f0\"");
		hasChildNodes = true;
	}

	if (fieldType->isOfType(structMetaObject))
	{
		WriteFormattedText(REF(writer), " typeSize=\"0x%02X\" align=\"0x%02X\"", ((Core::igStructMetaField*)metafield)->_typeSize, metafield->computeRequiredAlignment());
	}

	if (fieldType->isOfType(vectorMetaObject))
	{
		Core::igVectorMetaField* vectorMetaField = (Core::igVectorMetaField*)metafield;
		aint32_t memTypeAlign = vectorMetaField->_memTypeAlignment;
		if (memTypeAlign > 0)
		{
			Core::igObject* templateParam;
#if (TARGET_GAME > SKYSA_END) && (!TARGET_CAFE) && (!IS_GAME(SKYSA_WIIU)) // Vector metafield lacks getTemplateParameter method
			templateParam = metafield->getTemplateParameter(0);		
#else
			templateParam = vectorMetaField->_elementType ? static_cast<Core::igObject*>(vectorMetaField->_elementType) : static_cast<Core::igObject*>(vectorMetaField->_memType);
#endif // (TARGET_GAME > SKYSA_END) && (!TARGET_CAFE) && (!IS_GAME(SKYSA_WIIU))
			auint32_t memTypeSize;
			if (templateParam->getMeta()->isOfType(metaFieldMetaObject))
			{
				memTypeSize = reinterpret_cast<Core::igMetaField*>(templateParam)->computeSize();
			}
			else
			{
				// it's an igObjectRefMetaField
				memTypeSize = sizeof(Core::igObject*);
			}
			WriteFormattedText(REF(writer), " memTypeAlignmentMultiple=\"0x%02X\"", memTypeAlign / memTypeSize);
		}

		if (vectorMetaField->_initialCapacity > 0)
		{
			WriteFormattedText(REF(writer), " initialCapacity=\"%d\"", vectorMetaField->_initialCapacity);
		}
	}

	// Check for array metafields
	// Cursed way to do a logical AND
	if (Core::igMetaField* numField = fieldType->_metaFields.get(fieldType->_metaFields._count - 1))
	if (streq(numField->getName(), "_num"))
	{
		DEBUGPRINTF("Located num metafield at %08X, num field at %02X\n", numField, numField->_offset);
		WriteFormattedText(REF(writer), " num=\"%d\"", *(int*)(((asize_t)metafield) + numField->_offset))
	}

	const void* data = nullptr;
	if((aint32_t)metafield->_default._size < 0) data = &metafield->_default._buffer;
	else data = metafield->_default._buffer;

	#if TARGET_CAFE
	DEBUGPRINTF("Checking if we need the default! metafield: %s\n", metafield->getMeta()->getName());

	if ((data != nullptr) &&
		(data != (const void *)0x80000000) &&
		root &&
		(Core::igStringHelper::compare("igStaticMetaField", metafield->getMeta()->getName()) != 0)) // we don't dump a default value if it's a static field
	#else
	if ((data != nullptr))
	#endif // TARGET_CAFE
	{
		DEBUGPRINTF("Getting default! data: %p size: %d\n", data, metafield->_default._size);
		const char* defaultString = metafield->getStringFromMemory(data, 0);
		WriteFormattedText(REF(writer), " default=\"%s\"", FIX_STRING(defaultString));
		ReleaseString(defaultString);
	}

	auint32_t templateParamCount;
#if (TARGET_GAME > SKYSA_END) && (!TARGET_CAFE) && (!IS_GAME(SKYSA_WIIU))
	templateParamCount = metafield->getTemplateParameterCount();
#else
	templateParamCount = fieldType->isOfType(vectorMetaObject) ? 1 : 0;
#endif // (TARGET_GAME > SKYSA_END) && (!TARGET_CAFE) && (!IS_GAME(SKYSA_WIIU))

	hasChildNodes = hasChildNodes
	                || templateParamCount > 0;

	if (hasChildNodes)
	{
		writer.WriteText(2, ">\n");

		if (templateParamCount > 0)
		{
			WriteIndentedText(REF(writer), indent+1, 15, "<templateargs>\n");

			for (int i = 0; i < templateParamCount; i++)
			{
				Core::igObject* param;
#if (TARGET_GAME > SKYSA_END) && (!TARGET_CAFE) && (!IS_GAME(SKYSA_WIIU))
				param = metafield->getTemplateParameter(i);
#else // TARGET_GAME > SKYSA_END
				Core::igVectorMetaField* vectorMetaField = reinterpret_cast<Core::igVectorMetaField*>(metafield);
				param = vectorMetaField->_elementType ? static_cast<Core::igObject*>(vectorMetaField->_elementType) : static_cast<Core::igObject*>(vectorMetaField->_memType);
#endif // TARGET_GAME > SKYSA_END
				if (param == nullptr)
				{
					WriteIndentedText(REF(writer), indent+2, 8, "<null/>\n");
					continue;
				}

				if (param->getMeta()->isOfType(metaFieldMetaObject))
				{
					DumpMetaField(writer, indent + 2, (Core::igMetaField*)param);
				}
				else
				{
					Core::igObjectRefMetaField hackyhacky;
					*reinterpret_cast<Core::igMetaField*>(&hackyhacky) = defaultMetaField;
					hackyhacky._vTable = objectRefMetaObject->_vTablePointer;
					hackyhacky._construct = false;
					hackyhacky._destruct = false;
					hackyhacky._reconstruct = false;
					hackyhacky._refCounted = true;
					hackyhacky._metaObject = (Core::igMetaObject*)param;
					DumpMetaField(writer, indent + 2, &hackyhacky);
				}
			}

			WriteIndentedText(REF(writer), indent+1, 16, "</templateargs>\n");
		}

		if (fieldType->isOfType(memRefHandleMetaObject))
		{
			if (((Core::igMemoryRefHandleMetaField*)metafield)->_memType)
			{
				DumpMetaField(writer, indent + 1, ((Core::igMemoryRefHandleMetaField*)metafield)->_memType);
			}
			else
			{
				Core::igMetaField hackyhacky = defaultMetaField;
				hackyhacky._vTable = ucharMetaObject->_vTablePointer;
				DumpMetaField(writer, indent + 1, &hackyhacky);
			}
		}

		if (fieldType->isOfType(memRefMetaObject))
		{
			if (((Core::igMemoryRefMetaField*)metafield)->_memType)
			{
				DumpMetaField(writer, indent + 1, ((Core::igMemoryRefMetaField*)metafield)->_memType);
			}
			else
			{
				Core::igMetaField hackyhacky = defaultMetaField;
				hackyhacky._vTable = ucharMetaObject->_vTablePointer;
				DumpMetaField(writer, indent + 1, &hackyhacky);
			}
		}

		if (fieldType->isOfType(bitFieldMetaObject))
		{
			DumpMetaField(writer, indent + 1, ((Core::igBitFieldMetaField*)metafield)->_assignmentMetaField);
		}

		if (fieldType->isOfType(staticMetaObject))
		{
			DumpMetaField(writer, indent + 1, ((Core::igStaticMetaField*)metafield)->_storageMetaField);
		}

		if (fieldType->isOfType(propertyMetaObject))
		{
			DumpMetaField(writer, indent + 1, ((Core::igPropertyFieldMetaField*)metafield)->_innerMetaField);
		}

		WriteIndentedText(REF(writer), indent, 13, "</metafield>\n");
	}
	else
	{
		writer.WriteText(3, "/>\n");
	}
}

void DumpMetaObject(FileWriter& writer, Core::igMetaObject* meta)
{
	char buf[512];
	int len;
	
	DEBUGPRINTF("Dumping metaobject %p\n", meta);

	// temporarily removing static as a workaround since static constructors don't work on CemuPatchCompiler, TODO find a way to make these static again
	const Core::igMetaObject* dotnetObjectType           = Core::igArkCore_getObjectMeta(ArkCore, "igDotNetMetaObject");
	const Core::igMetaObject* dynamicObjectType          = Core::igArkCore_getObjectMeta(ArkCore, "igDotNetDynamicMetaObject");
	const Core::igMetaObject* dataListMetaObject         = Core::igArkCore_getObjectMeta(ArkCore, "igDataList");
	const Core::igMetaObject* objectListMetaObject       = Core::igArkCore_getObjectMeta(ArkCore, "igObjectList");
	const Core::igMetaObject* nrcObjectListMetaObject    = Core::igArkCore_getObjectMeta(ArkCore, "igNonRefCountedObjectList");
	const Core::igMetaObject* hashTableMetaObject        = Core::igArkCore_getObjectMeta(ArkCore, "igHashTable");
	const Core::igMetaObject* compoundFieldMetaObject    = Core::igArkCore_getObjectMeta(ArkCore, "igCompoundMetaField");
	const Core::igMetaObject* scriptGroupStackMetaObject = Core::igArkCore_getObjectMeta(ArkCore, "ScriptGroupStack");
	const Core::igMetaObject* rhsValueStackMetaObject    = Core::igArkCore_getObjectMeta(ArkCore, "RHSValueStack");
	const Core::igMetaObject* staticMetaFieldMetaObject  = Core::igArkCore_getObjectMeta(ArkCore, "igStaticMetaField");

	// Lots of types are generated from VVL scripts, this filters them
	// out. If you wanna get that type information you should parse the
	// scripts instead.
	if (dynamicObjectType
		&& meta->getMeta()->isOfType(dynamicObjectType))
	{
		return;
	}

	// Hack to let us dump metaobjects such that base types are dumped first
	// No object should have a size that's not aligned to a pointer so yeah
	if (meta->_sizeofSize & 1)
	{
		return;
	}
	meta->_sizeofSize |= 1;
	if (meta->_parent != nullptr)
	{
		DumpMetaObject(writer, meta->_parent);
	}

	DEBUGPRINTF("dumping metaobject %s\n", meta->getName());

	WriteFormattedText(REF(writer), "\t<metaobject type=\"%s\" refname=\"%s\"", meta->getMeta()->getName(), meta->getName());
	if (meta->_parent)
	{
		WriteFormattedText(REF(writer), " basetype=\"%s\"", meta->_parent->getName());
	}
	writer.WriteText(2, ">\n");

	int fieldStart = 0;

	if (meta->_parent)
	{
		int overrideFieldsCount = 0;
		for (int i = 0; i < meta->_parent->_metaFields._count; i++)
		{
			if (meta->_parent->_metaFields.get(i) == meta->_metaFields.get(i))
			{
				continue;
			}

			if (overrideFieldsCount == 0)
			{
				writer.WriteText(25, "\t\t<overriddenmetafields>\n");
			}
			overrideFieldsCount++;
			DEBUGPRINTF("dumping metafield %s::%s\n", meta->getName(), meta->_metaFields.get(i)->getName());
			DumpMetaField(writer, 3, meta->_metaFields.get(i), true);
		}
		if (overrideFieldsCount > 0)
		{
			writer.WriteText(26, "\t\t</overriddenmetafields>\n");
		}

		fieldStart = meta->_parent->_metaFields._count;

		// No need to handle igDataList as the overriden fields handle that
		if ((meta->isOfType(objectListMetaObject)    && meta != objectListMetaObject)
			|| (meta->isOfType(nrcObjectListMetaObject) && meta != nrcObjectListMetaObject))
		{
			Core::igMetaObject* elementType = 0;

			// These will crash if you call getElementType as they actually reference
			// member variables, instead just hardcode them to the common base object
			if (meta->isOfType(rhsValueStackMetaObject))
			{
				elementType = Core::igArkCore_getObjectMeta(ArkCore, "AbstractScriptVariant");
			}
			else if (meta->isOfType(scriptGroupStackMetaObject))
			{
				elementType = Core::igArkCore_getObjectMeta(ArkCore, "tfbScriptObject");
			}
			else
			{
				#if TARGET_CAFE
				DEBUGPRINTF("TARGET_CAFE!!!! We found a igTObjectList!!!\n",0);
				const char* metaName = meta->getName();
				int len = Core::igStringHelper::length(metaName);

				// hardcoded ones, the curse of not having getElementType on Wii U
				if (Core::igStringHelper::compare("AnimationStack", metaName) == 0)
				{
					elementType = Core::igArkCore_getObjectMeta(ArkCore, "AbstractAnimSequence");
				}
				else if (Core::igStringHelper::compare("igDirectory", metaName) == 0)
				{
					elementType = Core::igArkCore_getObjectMeta(ArkCore, "igDirEntry");
				}
				else if (Core::igStringHelper::compare("igNonRefCountedAttrList", metaName) == 0)
				{
					elementType = Core::igArkCore_getObjectMeta(ArkCore, "igAttr");
				}
				else if (Core::igStringHelper::compare("ValueStack", metaName) == 0)
				{
					elementType = Core::igArkCore_getObjectMeta(ArkCore, "tfbScriptObject");
				}
				else if (Core::igStringHelper::compare("PlacementSetList", metaName) == 0)
				{
					elementType = Core::igArkCore_getObjectMeta(ArkCore, "PlacementList");
				}
				else if (Core::igStringHelper::compare("SplashList", metaName) == 0)
				{
					elementType = Core::igArkCore_getObjectMeta(ArkCore, "SplashData");
				}
				else if (Core::igStringHelper::compare("igNonRefCountedAttrStackManagerList", metaName) == 0)
				{
					elementType = Core::igArkCore_getObjectMeta(ArkCore, "igAttrStackManager");
				}
				else if (Core::igStringHelper::compare("VectorStack", metaName) == 0)
				{
					elementType = Core::igArkCore_getObjectMeta(ArkCore, "tfbScriptObject");
				}
				else if (Core::igStringHelper::compare("ReferenceStack", metaName) == 0)
				{
					elementType = Core::igArkCore_getObjectMeta(ArkCore, "tfbScriptObject");
				}
				else if (Core::igStringHelper::compare("SoundList", metaName) == 0)
				{
					elementType = Core::igArkCore_getObjectMeta(ArkCore, "AbstractSoundSequence");
				}
				else if (Core::igStringHelper::compare("ModelSoundList", metaName) == 0)
				{
					elementType = Core::igArkCore_getObjectMeta(ArkCore, "AbstractSoundSequence");
				}
				else if (Core::igStringHelper::compare("PositionStack", metaName) == 0)
				{
					elementType = Core::igArkCore_getObjectMeta(ArkCore, "tfbScriptObject");
				}
				else if (Core::igStringHelper::compare("ScriptVariantList", metaName) == 0)
				{
					elementType = Core::igArkCore_getObjectMeta(ArkCore, "AbstractScriptVariant");
				}
				else if (Core::igStringHelper::compare("OpDefineMacroStack", metaName) == 0)
				{
					elementType = Core::igArkCore_getObjectMeta(ArkCore, "tfbScriptObject");
				}
				else if (Core::igStringHelper::compare("TagList", metaName) == 0)
				{
					elementType = Core::igArkCore_getObjectMeta(ArkCore, "AbstractScriptVariant");
				}
				else if (Core::igStringHelper::compare("tfbStreamMemoryConfigurationRefList", metaName) == 0)
				{
					elementType = Core::igArkCore_getObjectMeta(ArkCore, "tfbStreamMemoryConfiguration");
				}
				else if (Core::igStringHelper::compare("SetStack", metaName) == 0)
				{
					elementType = Core::igArkCore_getObjectMeta(ArkCore, "tfbScriptObject");
				}
				else if (Core::igStringHelper::compare("igNonRefCountedMetaImageList", metaName) == 0)
				{
					elementType = Core::igArkCore_getObjectMeta(ArkCore, "igMetaImage");
				}
				else if (Core::igStringHelper::compare("igIGBFile", metaName) == 0)
				{
					elementType = Core::igArkCore_getObjectMeta(ArkCore, "igDirEntry");
				}
				else if (Core::igStringHelper::compare("igNonRefCountedNodeList", metaName) == 0)
				{
					elementType = Core::igArkCore_getObjectMeta(ArkCore, "igNode");
				}
				else if (Core::igStringHelper::compare("tfbRendererList", metaName) == 0)
				{
					elementType = Core::igArkCore_getObjectMeta(ArkCore, "tfbSceneRenderer");
				}
				else if (Core::igStringHelper::compare("igNonRefCountedMemoryPoolList", metaName) == 0)
				{
					elementType = Core::igArkCore_getObjectMeta(ArkCore, "igMemoryPool");
				}
				else if (Core::igStringHelper::compare("igPoolHandleTable", metaName) == 0)
				{
					elementType = Core::igArkCore_getObjectMeta(ArkCore, "igMemoryPool");
				}
				else if (Core::igStringHelper::compare("igLightList", metaName) == 0)
				{
					elementType = Core::igArkCore_getObjectMeta(ArkCore, "igLightAttr");
				}
				else if (Core::igStringHelper::compare("BehaviorStack", metaName) == 0)
				{
					elementType = Core::igArkCore_getObjectMeta(ArkCore, "OpUserBehavior");
				}


				else if ((len <= 4) ||
					(metaName[len - 4] == 'L') &&
					(metaName[len - 3] == 'i') &&
					(metaName[len - 2] == 's') &&
					(metaName[len - 1] == 't'))
				{
					DEBUGPRINTF("The List check didn't fail!! about to copy it to the stack with copyn\n",0);
					char strippedString[128]; // hopefully big enough for every case
					Core::igStringHelper::copyn(metaName, strippedString, len - 3);
					DEBUGPRINTF("We just ran copyn, the string it made: %s\n", strippedString);
					elementType = Core::igArkCore_getObjectMeta(ArkCore, strippedString);

					// if simply stripping "List" at the end gives us an unknown type then we try to add
					// "tfb" as a prefix. If that doesn't work we cry and start hardcoding.
					if (elementType == nullptr)
					{
						strippedString[0] = 't';
						strippedString[1] = 'f';
						strippedString[2] = 'b';
						Core::igStringHelper::copyn(metaName, strippedString + 3, len - 3);
						DEBUGPRINTF("We just ran copyn, the string it made: %s\n", strippedString);

						elementType = Core::igArkCore_getObjectMeta(ArkCore, strippedString);
						if (elementType == nullptr)
						{
							_igReportPrintf("[FAIL] igTObjectList type name is weird: '%s' (len: %d)\n", metaName, len);
						}
					}

					DEBUGPRINTF("We just set the elementType\n",0);
					DEBUGPRINTF("elementType: %p\n", elementType);
					DEBUGPRINTF("elementType name: %s\n", elementType->getName());
				}

				else
				{
					_igReportPrintf("[FAIL] igTObjectList type name is weird: '%s' (len: %d)\n", metaName, len);
				}


				#else // TARGET_CAFE

				// This calls getElementType, which returns the igMetaObject
				// of the object this list contains
				//
				// the _data field of this igObjectList claims that the memory
				// contains igObjects, rather than the actual element type.
				elementType = ((Core::igMetaObject*(*)(Core::igObjectList*))GetVirtualFunc(meta->_vTablePointer, Core::igObjectList::kVTIndex_getElementType))(0);
			
				#endif // TARGET_CAFE
			}

			DEBUGPRINTF("We're about to write the elementType\n",0);
			WriteFormattedTextIndented(REF(writer), 2, "<objectlist elementtype=\"%s\"/>\n", elementType->getName());
			DEBUGPRINTF("We wrote the elementType\n",0);
		}
#if (TARGET_GAME > SKYSA_END) && (!IS_GAME(SKYSA_WIIU)) // Temporary - idk how hashtables work in ssa
		else if (meta->isOfType(hashTableMetaObject) && meta != hashTableMetaObject)
		{
			// Hashtables are a lil funky and have invalid keys/values, this
			// invalid key/value can change depending on the exact hash table
			// and messing up the invalid keys can break the hashtable.

			Core::igMemoryRefMetaField* valuesField = (Core::igMemoryRefMetaField*)meta->_metaFields.get(0);
			Core::igMemoryRefMetaField* keysField   = (Core::igMemoryRefMetaField*)meta->_metaFields.get(1);

			void* invalidKey   = ((void*(*)(Core::igHashTableVirtuals*))GetVirtualFunc(meta->_vTablePointer, Core::igHashTableVirtuals::kVTIndex_keyTraitsInvalid))(0);
			void* invalidValue = ((void*(*)(Core::igHashTableVirtuals*))GetVirtualFunc(meta->_vTablePointer, Core::igHashTableVirtuals::kVTIndex_valueTraitsInvalid))(0);

			WriteFormattedTextIndented(REF(writer),
										2,
										"<hashtable invalidvalue=\"%s\" invalidkey=\"%s\"/>\n",
										valuesField->_memType->getStringFromMemory(invalidValue, 0),
										keysField->_memType->getStringFromMemory(invalidKey, 0)
										);
		}
#endif // TARGET_GAME > SKYSA_END
	}

	if (meta->_metaFields._count - fieldStart > 0)
	{
		writer.WriteText(15, "\t\t<metafields>\n");
	}
	for (int i = fieldStart; i < meta->_metaFields._count; i++)
	{
		DEBUGPRINTF("dumping metafield %s::%s\n", meta->getName(), meta->_metaFields.get(i)->getName());
		DumpMetaField(writer, 3, meta->_metaFields.get(i), true);
	}
	if (meta->_metaFields._count - fieldStart > 0)
	{
		writer.WriteText(16, "\t\t</metafields>\n");
	}

#if TARGET_GAME >= SKYIM_01_00_00 // Issue #3 - Add exposed dotnet field info
	// Extra dotnet data
	if (meta->getMeta()->isOfType(dotnetObjectType))
	{
		DotNet::igDotNetMetaObject* dotnetMeta = reinterpret_cast<DotNet::igDotNetMetaObject*>(meta);
		const char** cppFieldNames = reinterpret_cast<const char**>(dotnetMeta->_cppFieldNames);
		const char** dnFieldNames = reinterpret_cast<const char**>(dotnetMeta->_dotNetFieldNames);

		if (dotnetMeta->_exposedFieldCount > 0)
		{
			writer.WriteText(17, "\t\t<dotnetfields>\n");
			for (aint32_t i = 0; i < dotnetMeta->_exposedFieldCount; i++)
			{
				WriteFormattedTextIndented(REF(writer),
										3,
										"<field cppName=\"%s\" dnName=\"%s\"/>\n",
										cppFieldNames[i],
										dnFieldNames[i]);
			}
			writer.WriteText(18, "\t\t</dotnetfields>\n");
		}
	}
#endif // TARGET_GAME >= SKYIM_01_00_00

	// Only check the direct parent, otherwise array metafields will cause trouble
	if (meta->_parent == compoundFieldMetaObject)
	{
		writer.WriteText(19, "\t\t<compoundfields>\n");

		Core::igCompoundMetaField* genericCompound = ((Core::igCompoundMetaField*(*)(Core::igMetaField*))GetVirtualFunc(meta->_vTablePointer, Core::igMetaField::kVTIndex_getGenericMetaField))(0);

		for (int i = 0; i < genericCompound->_fieldList->_count; i++)
		{
			DumpMetaField(writer, 3, genericCompound->_fieldList->get(i), true);
		}

		writer.WriteText(20, "\t\t</compoundfields>\n");
	}

	writer.WriteText(15, "\t</metaobject>\n");
}

void DumpMetaObjects()
{
	FileWriter writer = FileWriter("metaobjects.xml");
	writer.WriteText(14, "<metaobjects>\n");

	DEBUGPRINTF("ArkCore at %p\n", ArkCore);
	DEBUGPRINTF("ArkCore->_metaObjectHashTable at %p\n", ArkCore->_metaObjectHashTable);
	DEBUGPRINTF("ArkCore->_metaObjectHashTable->_values at %p\n", ArkCore->_metaObjectHashTable->_values._buffer);
	Core::igMemory<Core::igMetaObject*>& metaObjects = ArkCore->_metaObjectHashTable->_values;
	for (int i = 0; i < metaObjects._size / sizeof(Core::igMetaObject*); i++)
	{
		Core::igMetaObject* meta = metaObjects.get(i);

		// We're iterating over a hashtable, it can have invalid values,
		// check for those
		if (meta == nullptr)
		{
			continue;
		}

		DEBUGPRINTF("At address %p with index %d with metaobject %p\n", metaObjects._buffer + i, i, meta);

		DumpMetaObject(writer, meta);
	}

	writer.WriteText(14, "</metaobjects>");
}

#if IS_GAME(SKYSA) || IS_GAME(SKYTT) || IS_GAME(SKYSA_WIIU) // tfbScript bindings
void DumpTfbBindings()
{
	char buf[512];
	int len;

	Core::igMemory<Core::igHandle>* handles = &igObjectHandleManagerInstance->_handleTable->_values;
	auint32_t handleCount = (handles->_size & 0x00FFFFFF) / sizeof(Core::igHandle*);

	Core::igTDataList<const char*>* systemNamespaces = igObjectHandleManagerInstance->_systemNamespaces;

	FileWriter writer = FileWriter("tfbbindings.xml");
	writer.WriteText(14, "<tfbbindings>\n");

	DEBUGPRINTF("handle num is %d\n", handleCount);
	DEBUGPRINTF("handle hash item count is %d\n", igObjectHandleManagerInstance->_handleTable->_hashItemCount);
	for (int i = 0; i < handleCount; i++)
	{
		Core::igHandle handle = handles->get(i);
		if (!handle._data)
		{
			continue;
		}

		const char* nameSpace = handle._data->_namespace._name;
		bool isSystemNamespace = false;
		for (int s = 0; s < systemNamespaces->_count; s++)
		{
			if (streq(nameSpace, systemNamespaces->get(s)))
			{
				isSystemNamespace = true;
				break;
			}
		}

		bool excludedSystemNamespace = isSystemNamespace;
		excludedSystemNamespace &= streq(nameSpace, "metaimages")
		                        || streq(nameSpace, "vertexformat")
		                        || streq(nameSpace, "vertexblender")
		                        || streq(nameSpace, "indexformats");

		const char* typeName = "(null)";
		if (handle._data->_object)
		{
			typeName = handle._data->_object->getMeta()->getName();
		}

		if (!excludedSystemNamespace && isSystemNamespace)
		{
			WriteFormattedTextIndented(REF(writer), 1, "<binding type=\"%s\" name=\"%s.%s\"/>\n", typeName, FIX_STRING(handle._data->_namespace._name), FIX_STRING(handle._data->_name._name));
		}
	}

	writer.WriteText(15, "</tfbbindings>\n");
}
#endif // IS_GAME(SKYSA) || IS_GAME(SKYTT) || IS_GAME(SKYSA_WIIU) 

void MetadataDumperThread()
{
	_igReportPrintf("hai\n");

	asleep(10000);

	_igReportPrintf("dumping metadata now\n");

	defaultMetaField._vTable = 0;
	defaultMetaField._properties._copyMethod = 3;
	defaultMetaField._properties._resetMethod = 3;
	defaultMetaField._properties._isAlikeMethod = 3;
	defaultMetaField._properties._itemsCopyMethod = 3;
	defaultMetaField._properties._keysCopyMethod = 3;
	defaultMetaField._properties._requiredAlignment = 1;
	defaultMetaField._properties._persistent = 1;
	defaultMetaField._properties._hasInvariance = 0;
	defaultMetaField._properties._hasPoolName = 0;
	defaultMetaField._properties._mutable = 0;
	defaultMetaField._properties._implicitAlignment = 1;
	defaultMetaField._default._buffer = 0;
	defaultMetaField._default._size = 0;

	FileWriter writer = FileWriter("metaenums.xml");
	SetupMetaenumDumping(writer);

	// Order is essential
	DumpMetaObjects();     // Generates some of the metaenums when we call _getMetaEnum

#if TARGET_GAME > SKYSA_END // SSA lacks a global metaenum list
	DumpMetaEnums(writer); // Grabs the platform enum
#endif // TARGET_GAME > SKYSA_END

#if TARGET_GAME > SKYSA_END // SSA lacks the computePlatformAlignment and computePlatformSize methods
	DumpMetaFieldList();
#endif // TARGET_GAME > SKYSA_END

#if IS_GAME(SKYSA) || IS_GAME(SKYTT) || IS_GAME(SKYSA_WIIU) 
	DumpTfbBindings();
#endif // IS_GAME(SKYSA) || IS_GAME(SKYTT) || IS_GAME(SKYSA_WIIU) 

	StopMetaenumDumping();

	_igReportPrintf("\nall done!\n");
}