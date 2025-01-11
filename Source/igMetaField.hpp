#pragma once

#include "Defines.h"

#include "igObject.hpp"
#include "igObjectList.hpp"
#include "igStringRef.hpp"

namespace Core
{
	class igMetaEnum;

	struct fieldProperties
	{
		static const char* sPropertyTypeStrings[];

		uint32_t _copyMethod : 2;
		uint32_t _resetMethod : 2;
		uint32_t _isAlikeMethod : 2;
		uint32_t _itemsCopyMethod : 2;
		uint32_t _keysCopyMethod : 2;
		uint32_t _requiredAlignment : 8;
		uint32_t _persistent : 1;
		uint32_t _hasInvariance : 1;
		uint32_t _hasPoolName : 1;
		uint32_t _mutable : 1;
		uint32_t _implicitAlignment : 1;
	};


	class igMetaField : public igObject
	{
	public:
		uint16_t _parentMetaObjectIndex;	//0x08
		int16_t _typeIndex;					//0x0A
		int16_t _internalIndex;				//0x0C
		uint16_t _size;						//0x0E
		uint16_t _offset;					//0x10
		igObjectList* _attributes;			//0x14
		union
		{
			fieldProperties _properties;
			uint32_t _propertiesBitfield;
		};
		char * _fieldName;					//0x1C
		igMemory<uint8_t> _default;			//0x20

		inline const char* getStringFromMemory(void* memory, igObject* directory)
		{
			const char* string;

#if TARGET_PPC
			// PowerPC abi moment
			((const char*(*)(const char**, igMetaField*, void*, igObject*))GetVirtualFunc(_vTable, 0x45))(&string, this, memory, directory);
#else
#error "Platform not supported"
#endif // TARGET_PPC

			return string;
		}
		DefineVirtualFunc_0(0x16, igMetaField*, getGenericMetaField);
		DefineVirtualFunc_0(0x1F, uint32_t, computeSize);
		DefineVirtualFunc_1(0x20, uint32_t, computePlatformSize, int32_t, platform);
		DefineVirtualFunc_0(0x23, uint32_t, computeRequiredAlignment);
		DefineVirtualFunc_1(0x24, uint32_t, computePlatformAlignment, int32_t, platform);
		//DefineVirtualFunc_2(0x45, igStringRef, getStringFromMemory, void*, memory, igObjectDirectory*, directory);
		DefineVirtualFunc_0_c(0x52, int, getTemplateParameterCount);
		DefineVirtualFunc_1(0x53, igObject*, getTemplateParameter, int, i);
	};

	class igRefMetaField : public igMetaField
	{
	public:
		bool _construct;
		bool _destruct;
		bool _reconstruct;
		bool _refCounted;
	};

	class igMemoryRefMetaField : public igRefMetaField
	{
	public:
		int32_t _memSize;
		igMetaField* _memType;
		int32_t _memTypeAlignment;
		igMetaField* _memTypeRef;
		bool _releaseOnCopy;
		bool _releaseOnReset;
	};

	class igMemoryRefHandleMetaField : public igRefMetaField
	{
	public:
		igMetaField* _memType;
	};

	class igObjectRefMetaField : public igRefMetaField
	{
	public:
		igMetaObject* _metaObject;
	};

	class igHandleMetaField : public igRefMetaField
	{
	public:
		igMetaObject* _metaObject;
	};

	class igBitFieldMetaField : public igMetaField
	{
	public:
		uint32_t _shift;
		uint32_t _bits;
		igMetaField* _storageMetaField;
		igMetaField* _assignmentMetaField;
	};

	class igEnumMetaField : public igMetaField
	{
	public:
		igMetaEnum*(*_getMetaEnumFunction)();
	};

	class igPropertyFieldMetaField : public igMetaField
	{
	public:
		void* _setCallbackFunction;
		void* _getCallbackFunction;
		igMetaField* _innerMetaField;
	};

	class igStaticMetaField : public igMetaField
	{
	public:
		igMetaField* _storageMetaField;
		void* _staticPointer;
		igMetaObject* _owner;
	};

	class igCompoundMetaField : public igMetaField
	{
	public:
		igTObjectList<igMetaField>* _fieldList;
	};

	class igStructMetaField : public igMetaField
	{
	public:
		uint16_t _typeSize;
		void* _cppConstructor;
		void* _cppDestructor;
	};

	class igVectorMetaField : public igRefMetaField
	{
	public:
		igMetaField* _memType;
		igMetaField* _memTypeRef;
		int32_t _memTypeAlignment;
		igMetaObject* _elementType;
		int32_t _initialCapacity;
	};
}