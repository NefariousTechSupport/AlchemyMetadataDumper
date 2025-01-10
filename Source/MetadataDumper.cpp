#include "MetadataDumper.hpp"

#include "Addresses.h"

#include "igArkCore.hpp"
#include "igObjectList.hpp"
#include "igHashTable.hpp"
#include "igMetaEnum.hpp"
#include "igMetaObject.hpp"
#include "igMetaField.hpp"

#include "cpp11hacks.hpp"
#include "fileio.hpp"

#define DEBUG_LOGS 0

#if DEBUG_LOGS
#define DEBUGPRINTF(fmt, ...) _igReportPrintf(fmt, __VA_ARGS__)
#else
#define DEBUGPRINTF(fmt, ...)
#endif // DEBUG_LOGS

bool streq(const char* a, const char* b)
{
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

void DumpMetaEnums()
{
	char buf[512];
	int len;

	Core::igMetaObject* dynamicMetaEnumType = Core::igArkCore_getObjectMeta(ArkCore, "igDotNetDynamicMetaEnum");

	FileWriter writer = FileWriter(FILE_PREFIX "metaenums.xml");

	writer.WriteText(12, "<metaenums>\n");

	Core::igMemory<Core::igMetaEnum*>& metaEnums = ArkCore->_metaEnumHashTable->_values;
	for(int i = 0; i < metaEnums._size / sizeof(Core::igMetaEnum*); i++)
	{
		Core::igMetaEnum* metaEnum = metaEnums.get(i);
		if(metaEnum == nullptr)
		{
			continue;
		}

		if(dynamicMetaEnumType && metaEnum->getMeta()->isOfType(dynamicMetaEnumType))
		{
			continue;
		}

		if (!platformMetaEnum && streq(metaEnum->_name, "IG_CORE_PLATFORM"))
		{
			platformMetaEnum = metaEnum;
		}
		
		WriteFormattedText(writer, "\t<metaenum refname=\"%s\">\n", metaEnum->_name);
		for(int j = 0; j < metaEnum->_names->_count; j++)
		{
			WriteFormattedText(writer, "\t\t<value name=\"%s\" value=\"%d\"/>\n", metaEnum->_names->get(j), metaEnum->_values->get(j));
		}
		WriteFormattedText(writer, "\t</metaenum>\n", metaEnum->_name);
	}
	writer.WriteText(12, "</metaenums>");
}

void DumpMetaFieldList()
{
	char buf[512];
	int len;

	FileWriter writer = FileWriter(FILE_PREFIX "metafields.xml");
	writer.WriteText(13, "<metafields>\n");

	for (int t = 0; t < ArkCoreMetaFieldList->_count; t++)
	{
		Core::igMetaField* field = ArkCoreMetaFieldList->get(t);
		WriteFormattedTextIndented(writer, 1, "<metafield name=\"%s\">\n", field->getMeta()->_name);

		for (int i = 0; i < platformMetaEnum->_names->_count; i++)
		{
			const char* platformName = platformMetaEnum->_names->get(i);

			if (streq(platformName, "IG_CORE_PLATFORM_DEFAULT")
			 || streq(platformName, "IG_CORE_PLATFORM_DEPRECATED")
			 || streq(platformName, "IG_CORE_PLATFORM_MAX"))
			{
				continue;
			}

			WriteFormattedTextIndented(writer, 2, "<platforminfo platform=\"%s\" align=\"0x%02X\" size=\"0x%02X\"/>\n", platformName, field->computePlatformAlignment(i), field->computePlatformSize(i));
		}

		WriteIndentedText(writer, 1, 13, "</metafield>\n");
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

	static const Core::igMetaObject* metaFieldMetaObject    = Core::igArkCore_getObjectMeta(ArkCore, "igMetaField");
	static const Core::igMetaObject* memRefMetaObject       = Core::igArkCore_getObjectMeta(ArkCore, "igMemoryRefMetaField");
	static const Core::igMetaObject* memRefHandleMetaObject = Core::igArkCore_getObjectMeta(ArkCore, "igMemoryRefHandleMetaField");
	static const Core::igMetaObject* refMetaObject          = Core::igArkCore_getObjectMeta(ArkCore, "igRefMetaField");
	static const Core::igMetaObject* objectRefMetaObject    = Core::igArkCore_getObjectMeta(ArkCore, "igObjectRefMetaField");
	static const Core::igMetaObject* handleMetaObject       = Core::igArkCore_getObjectMeta(ArkCore, "igHandleMetaField");
	static const Core::igMetaObject* bitFieldMetaObject     = Core::igArkCore_getObjectMeta(ArkCore, "igBitFieldMetaField");
	static const Core::igMetaObject* enumMetaObject         = Core::igArkCore_getObjectMeta(ArkCore, "igEnumMetaField");
	static const Core::igMetaObject* staticMetaObject       = Core::igArkCore_getObjectMeta(ArkCore, "igStaticMetaField");
	static const Core::igMetaObject* propertyMetaObject     = Core::igArkCore_getObjectMeta(ArkCore, "igPropertyFieldMetaField");
	static const Core::igMetaObject* ucharMetaObject        = Core::igArkCore_getObjectMeta(ArkCore, "igUnsignedCharMetaField");
	static const Core::igMetaObject* structMetaObject       = Core::igArkCore_getObjectMeta(ArkCore, "igStructMetaField");

	//_igReportPrintf("memref is null? %d \n", memRefMetaObject == nullptr ? 1 : 0);

	bool hasChildNodes = false;


	WriteFormattedTextIndented(writer,
	                           indent,
	                           "<metafield type=\"%s\""
	                           " offset=\"0x%04X\"",
	                           metafield->getMeta()->_name,
	                           metafield->_offset
	                           );

	if (root && metafield->_fieldName)
	{
		WriteFormattedText(writer, " name=\"%s\"", metafield->_fieldName);
	}

	if (metafield->_properties._copyMethod != 3)
	{
		WriteFormattedText(writer, " copyMethod=\"%s\"", Core::fieldProperties::sPropertyTypeStrings[metafield->_properties._copyMethod]);
	}
	if (metafield->_properties._resetMethod != 3)
	{
		WriteFormattedText(writer, " resetMethod=\"%s\"", Core::fieldProperties::sPropertyTypeStrings[metafield->_properties._resetMethod]);
	}
	if (metafield->_properties._isAlikeMethod != 3)
	{
		WriteFormattedText(writer, " isAlikeMethod=\"%s\"", Core::fieldProperties::sPropertyTypeStrings[metafield->_properties._isAlikeMethod]);
	}
	if (metafield->_properties._itemsCopyMethod != 3)
	{
		WriteFormattedText(writer, " itemsCopyMethod=\"%s\"", Core::fieldProperties::sPropertyTypeStrings[metafield->_properties._itemsCopyMethod]);
	}
	if (metafield->_properties._keysCopyMethod != 3)
	{
		WriteFormattedText(writer, " keysCopyMethod=\"%s\"", Core::fieldProperties::sPropertyTypeStrings[metafield->_properties._keysCopyMethod]);
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
	}


	// Do not if/else this, only if
	// e.g., igRefMetaField can be a base class for several of the ones below

	if (fieldType->isOfType(refMetaObject))
	{
		WriteFormattedText(writer, " construct=\"%s\" destruct=\"%s\" reconstruct=\"%s\" refCounted=\"%s\"",
			((Core::igRefMetaField*)metafield)->_construct   ? "true" : "false",
			((Core::igRefMetaField*)metafield)->_destruct    ? "true" : "false",
			((Core::igRefMetaField*)metafield)->_reconstruct ? "true" : "false",
			((Core::igRefMetaField*)metafield)->_refCounted  ? "true" : "false"
			);
	}

	if (fieldType->isOfType(objectRefMetaObject))
	{
		WriteFormattedText(writer, " metaobject=\"%s\"", 
			((Core::igObjectRefMetaField*)metafield)->_metaObject ? ((Core::igObjectRefMetaField*)metafield)->_metaObject->_name : "igObject"
			);
	}

	if (fieldType->isOfType(handleMetaObject))
	{
		WriteFormattedText(writer, " metaobject=\"%s\"", 
			((Core::igHandleMetaField*)metafield)->_metaObject ? ((Core::igHandleMetaField*)metafield)->_metaObject->_name : "igObject"
			);
	}

	if (fieldType->isOfType(memRefMetaObject) || fieldType->isOfType(memRefHandleMetaObject))
	{
		writer.WriteText(13, " memType=\"f0\"");
		hasChildNodes = true;
	}

	if (fieldType->isOfType(bitFieldMetaObject))
	{
		WriteFormattedText(writer,
		                   " shift=\"0x%02X\" bits=\"0x%02X\" storageField=\"%s\" assignmentField=\"f0\"",
		                   ((Core::igBitFieldMetaField*)metafield)->_shift,
		                   ((Core::igBitFieldMetaField*)metafield)->_bits,
		                   ((Core::igBitFieldMetaField*)metafield)->_storageMetaField->_fieldName
		                   );
		hasChildNodes = true;
	}

	if (fieldType->isOfType(enumMetaObject))
	{
		Core::igMetaEnum*(*getMetaEnumFunc)() = ((Core::igEnumMetaField*)metafield)->_getMetaEnumFunction;
		WriteFormattedText(writer,
		                   " metaenum=\"%s\"",
		                   getMetaEnumFunc ? getMetaEnumFunc()->_name : "(null)"
		                   );
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
		WriteFormattedText(writer, " typeSize=\"0x%02X\" align=\"0x%02X\"", ((Core::igStructMetaField*)metafield)->_typeSize, metafield->computeRequiredAlignment());
	}

	// Check for array metafields
	// Cursed way to do a logical AND
	if (Core::igMetaField* numField = fieldType->_metaFields.get(fieldType->_metaFields._count - 1))
	if (streq(numField->_fieldName, "_num"))
	{
		DEBUGPRINTF("Located num metafield at %08X, num field at %02X\n", numField, numField->_offset);
		WriteFormattedText(writer, " num=\"%d\"", *(int*)(((size_t)metafield) + numField->_offset))
	}

	void* data = nullptr;
	if((int32_t)metafield->_default._size < 0) data = &metafield->_default._buffer;
	else data = metafield->_default._buffer;

	if (data != nullptr)
	{
		WriteFormattedText(writer, " default=\"%s\"", metafield->getStringFromMemory(data, 0));
	}

	hasChildNodes = hasChildNodes
	                || metafield->getTemplateParameterCount() > 0;

	if (hasChildNodes)
	{
		writer.WriteText(2, ">\n");

		if (metafield->getTemplateParameterCount() > 0)
		{
			WriteIndentedText(writer, indent+1, 15, "<templateargs>\n");

			for (int i = 0; i < metafield->getTemplateParameterCount(); i++)
			{
				Core::igObject* param = metafield->getTemplateParameter(i);
				if (param == nullptr)
				{
					WriteIndentedText(writer, indent+2, 8, "<null/>\n");
					continue;
				}

				if (param->getMeta()->isOfType(metaFieldMetaObject))
				{
					DumpMetaField(writer, indent + 2, (Core::igMetaField*)param);
				}
				else
				{
					Core::igObjectRefMetaField hackyhacky;
					hackyhacky._vTable = objectRefMetaObject->_vTablePointer;
					hackyhacky._properties._copyMethod = 3;
					hackyhacky._properties._resetMethod = 3;
					hackyhacky._properties._isAlikeMethod = 3;
					hackyhacky._properties._itemsCopyMethod = 3;
					hackyhacky._properties._keysCopyMethod = 3;
					hackyhacky._properties._requiredAlignment = sizeof(Core::igObjectRefMetaField*);
					hackyhacky._properties._persistent = 1;
					hackyhacky._properties._hasInvariance = 0;
					hackyhacky._properties._hasPoolName = 0;
					hackyhacky._properties._mutable = 0;
					hackyhacky._properties._implicitAlignment = 1;
					hackyhacky._default._buffer = 0;
					hackyhacky._default._size = 0;
					hackyhacky._construct = false;
					hackyhacky._destruct = false;
					hackyhacky._reconstruct = false;
					hackyhacky._refCounted = true;
					hackyhacky._metaObject = (Core::igMetaObject*)param;
					DumpMetaField(writer, indent + 2, &hackyhacky);
				}
			}

			WriteIndentedText(writer, indent+1, 16, "</templateargs>\n");
		}

		if (fieldType->isOfType(memRefHandleMetaObject))
		{
			if (((Core::igMemoryRefHandleMetaField*)metafield)->_memType)
			{
				DumpMetaField(writer, indent + 1, ((Core::igMemoryRefHandleMetaField*)metafield)->_memType);
			}
			else
			{
				Core::igMetaField hackyhacky;
				hackyhacky._vTable = ucharMetaObject->_vTablePointer;
				hackyhacky._properties._copyMethod = 3;
				hackyhacky._properties._resetMethod = 3;
				hackyhacky._properties._isAlikeMethod = 3;
				hackyhacky._properties._itemsCopyMethod = 3;
				hackyhacky._properties._keysCopyMethod = 3;
				hackyhacky._properties._requiredAlignment = 1;
				hackyhacky._properties._persistent = 1;
				hackyhacky._properties._hasInvariance = 0;
				hackyhacky._properties._hasPoolName = 0;
				hackyhacky._properties._mutable = 0;
				hackyhacky._properties._implicitAlignment = 1;
				hackyhacky._default._buffer = 0;
				hackyhacky._default._size = 0;
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
				Core::igMetaField hackyhacky;
				hackyhacky._vTable = ucharMetaObject->_vTablePointer;
				hackyhacky._properties._copyMethod = 3;
				hackyhacky._properties._resetMethod = 3;
				hackyhacky._properties._isAlikeMethod = 3;
				hackyhacky._properties._itemsCopyMethod = 3;
				hackyhacky._properties._keysCopyMethod = 3;
				hackyhacky._properties._requiredAlignment = 1;
				hackyhacky._properties._persistent = 1;
				hackyhacky._properties._hasInvariance = 0;
				hackyhacky._properties._hasPoolName = 0;
				hackyhacky._properties._mutable = 0;
				hackyhacky._properties._implicitAlignment = 1;
				hackyhacky._default._buffer = 0;
				hackyhacky._default._size = 0;
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

		WriteIndentedText(writer, indent, 13, "</metafield>\n");
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

	static const Core::igMetaObject* dotnetObjectType        = Core::igArkCore_getObjectMeta(ArkCore, "igDotNetMetaObject");
	static const Core::igMetaObject* dynamicObjectType       = Core::igArkCore_getObjectMeta(ArkCore, "igDotNetDynamicMetaObject");
	static const Core::igMetaObject* dataListMetaObject      = Core::igArkCore_getObjectMeta(ArkCore, "igDataList");
	static const Core::igMetaObject* objectListMetaObject    = Core::igArkCore_getObjectMeta(ArkCore, "igObjectList");
	static const Core::igMetaObject* nrcObjectListMetaObject = Core::igArkCore_getObjectMeta(ArkCore, "igNonRefCountedObjectList");
	static const Core::igMetaObject* hashTableMetaObject     = Core::igArkCore_getObjectMeta(ArkCore, "igHashTable");
	static const Core::igMetaObject* compoundFieldMetaObject = Core::igArkCore_getObjectMeta(ArkCore, "igCompoundMetaField");

	if (!dynamicObjectType)
	{
		_igReportPrintf("AAAAAA\n");
	}

	// Lots of types are generated from VVL scripts, this filters them
	// out. If you wanna get that type information you should parse the
	// scripts instead.
	if (dynamicObjectType
		&& meta->getMeta()->isOfType(dynamicObjectType))
	{
		return;
	}

	// Hack to let us dump metaobjects such that base types are dumped first
	// this makes the game crash shortly after the metadata dumper finishes running
	if (meta->_instanceCount < 0)
	{
		return;
	}
	meta->_instanceCount = -20000000;
	if (meta->_parent != nullptr)
	{
		DumpMetaObject(writer, meta->_parent);
	}

	DEBUGPRINTF("dumping metaobject %s\n", meta->_name);

	WriteFormattedText(writer, "\t<metaobject type=\"%s\" refname=\"%s\"", meta->getMeta()->_name, meta->_name);
	if (meta->_parent)
	{
		WriteFormattedText(writer, " basetype=\"%s\"", meta->_parent->_name);
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
			DEBUGPRINTF("dumping metafield %s::%s\n", meta->_name, meta->_metaFields.get(i)->_fieldName);
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
			// This calls getElementType, which returns the igMetaObject
			// of the object this list contains
			//
			// the _data field of this igObjectList claims that the memory
			// contains igObjects, rather than the actual element type.
			Core::igMetaObject* elementType = ((Core::igMetaObject*(*)(Core::igObjectList* thisPtr))GetVirtualFunc(meta->_vTablePointer, 0x1C))(0);

			WriteFormattedTextIndented(writer, 2, "<objectlist elementtype=\"%s\"/>\n", elementType->_name);
		}
		else if (meta->isOfType(hashTableMetaObject) && meta != hashTableMetaObject)
		{
			// Hashtables are a lil funky and have invalid keys/values, this
			// invalid key/value can change depending on the exact hash table
			// and messing up the invalid keys can break the hashtable.

			Core::igMemoryRefMetaField* valuesField = (Core::igMemoryRefMetaField*)meta->_metaFields.get(0);
			Core::igMemoryRefMetaField* keysField   = (Core::igMemoryRefMetaField*)meta->_metaFields.get(1);

			void* invalidValue = ((Core::igMetaObject*(*)(Core::igContainer* thisPtr))GetVirtualFunc(meta->_vTablePointer, 0x1E))(0);
			void* invalidKey = ((Core::igMetaObject*(*)(Core::igContainer* thisPtr))GetVirtualFunc(meta->_vTablePointer, 0x1B))(0);

			WriteFormattedTextIndented(writer,
										2,
										"<hashtable invalidvalue=\"%s\" invalidkey=\"%s\"/>\n",
										valuesField->_memType->getStringFromMemory(invalidValue, 0),
										keysField->_memType->getStringFromMemory(invalidKey, 0)
										);
		}
	}

	if (meta->_metaFields._count - fieldStart > 0)
	{
		writer.WriteText(15, "\t\t<metafields>\n");
	}
	for (int i = fieldStart; i < meta->_metaFields._count; i++)
	{
		DEBUGPRINTF("dumping metafield %s::%s\n", meta->_name, meta->_metaFields.get(i)->_fieldName);
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
			for (int32_t i = 0; i < dotnetMeta->_exposedFieldCount; i++)
			{
				WriteFormattedTextIndented(writer,
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

		Core::igCompoundMetaField* genericCompound = ((Core::igCompoundMetaField*(*)(Core::igMetaField*))GetVirtualFunc(meta->_vTablePointer, 0x16))(0);

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
	FileWriter writer = FileWriter(FILE_PREFIX "metaobjects.xml");
	writer.WriteText(14, "<metaobjects>\n");

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

		DumpMetaObject(writer, meta);
	}

	writer.WriteText(14, "</metaobjects>");
}

void MetadataDumperThread(uint64_t threadId)
{
	_igReportPrintf("hai\n");

	sleep(10000);

	_igReportPrintf("dumping metadata now\n");

	// Order is essential
	DumpMetaObjects();     // Generates some of the metaenums when we call _getMetaEnum
	DumpMetaEnums();       // Grabs the platform enum
	DumpMetaFieldList();

	_igReportPrintf("all done!\n");
}