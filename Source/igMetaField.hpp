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

		auint32_t _copyMethod : 2;
		auint32_t _resetMethod : 2;
		auint32_t _isAlikeMethod : 2;
		auint32_t _itemsCopyMethod : 2;
		auint32_t _keysCopyMethod : 2;
		auint32_t _requiredAlignment : 8;
		auint32_t _persistent : 1;
		auint32_t _hasInvariance : 1;
		auint32_t _hasPoolName : 1;
		auint32_t _mutable : 1;
		auint32_t _implicitAlignment : 1;
	};


	class igMetaField : public igObject
	{
	public:
#if TARGET_GAME >= SKYSA_WIIU_BEGIN
		auint16_t _parentMetaObjectIndex;   //0x08
		aint16_t _typeIndex;                //0x0A
		aint16_t _internalIndex;            //0x0C
		auint16_t _size;                    //0x0E
		auint16_t _offset;                  //0x10
		igObjectList* _attributes;          //0x14
		union
		{
			fieldProperties _properties;
			auint32_t _propertiesBitfield;
		};
	private:
		char* _fieldName;                   //0x1C
	public:
		igMemory<auint8_t> _default;        //0x20
#else
		union
		{
			fieldProperties _properties;
			auint32_t _propertiesBitfield;
		};
	private:
		char* _fieldName;                   //0x0C
	public:
		auint16_t _offset;                  //0x10
		auint16_t _parentMetaObjectIndex;   //0x12
		aint16_t _typeIndex;                //0x14
		aint16_t _internalIndex;            //0x16
		auint16_t _size;                    //0x18
		igObjectList* _attributes;          //0x1C
		igMemory<auint8_t> _default;        //0x20
#endif // TARGET_GAME >= SKYSA_WIIU_BEGIN

		inline const char* getName()
		{
			asize_t name = reinterpret_cast<asize_t>(_fieldName);
#if TARGET_WII // They use the most significant bit to store something on wii
			name = name | 0x80000000;
#endif // TARGET_WII
			return reinterpret_cast<const char*>(name);
		}

		inline const char* getStringFromMemory(const void* memory, igObject* directory)
		{
			const char* string;

#if TARGET_PPC && (!TARGET_CAFE)
			// PowerPC abi moment
			((const char*(*)(const char**, igMetaField*, const void*, igObject*))GetVirtualFunc(_vTable, kVTIndex_getStringFromMemory))(&string, this, memory, directory);
#elif TARGET_CAFE
			// the GHS abi puts a pointer to the return value after 'this' https://zenith.nsmbu.net/wiki/Green_Hills_Software/Application_Binary_Interface#Stack_Returns
			((void (*)(igMetaField*, const char**, const void*, igObject*))GetVirtualFunc(_vTable, kVTIndex_getStringFromMemory))(this, &string, memory, directory);
#else
#error "Platform not supported"
#endif // TARGET_PPC && (!TARGET_CAFE)

			return string;
		}
		DefineVirtualFunc_0(igMetaField*, getGenericMetaField);
		DefineVirtualFunc_0(auint32_t, computeSize);
		DefineVirtualFunc_1(auint32_t, computePlatformSize, aint32_t, platform);
		DefineVirtualFunc_0(auint32_t, computeRequiredAlignment);
		DefineVirtualFunc_1(auint32_t, computePlatformAlignment, aint32_t, platform);
		DeclareVirtualFunc(getStringFromMemory);
#if (TARGET_GAME > SKYSA_END) && (!TARGET_CAFE) && (!IS_GAME(SKYSA_WIIU))// template parameter was added later
		DefineVirtualFunc_0_c(int, getTemplateParameterCount);
		DefineVirtualFunc_1(igObject*, getTemplateParameter, int, i);
#endif // (TARGET_GAME > SKYSA_END) && (!TARGET_CAFE) && (!IS_GAME(SKYSA_WIIU))
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
		aint32_t _memSize;
		igMetaField* _memType;
		aint32_t _memTypeAlignment;
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
		auint32_t _shift;
		auint32_t _bits;
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

		DefineVirtualFunc_0(igCompoundMetaField*, getGenericMetaField)
	};

	class igStructMetaField : public igMetaField
	{
	public:
		auint16_t _typeSize;
		void* _cppConstructor;
		void* _cppDestructor;
	};

	class igVectorMetaField : public igRefMetaField
	{
	public:
		igMetaField* _memType;
		igMetaField* _memTypeRef;
		aint32_t _memTypeAlignment;
		igMetaObject* _elementType;
		aint32_t _initialCapacity;
	};
}