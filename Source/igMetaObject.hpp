#pragma once

#include "igBaseMeta.hpp"
#include "igObjectList.hpp"
#include "igVector.hpp"

namespace Core
{
	class igMetaField;

	class igMetaObject
#if TARGET_GAME >= SKYSA_WIIU_BEGIN
	: public igBaseMeta
#else
	: public igObject
#endif // TARGET_GAME >= SKYSA_WIIU_BEGIN
	{
	public:
#if TARGET_GAME >= SKYSA_WIIU_BEGIN
		igTVector<igMetaField*> _metaFields;
		aint32_t _instanceCount;
		void** _vTablePointer;
		igMetaObject* _parent;
		igObject* _lastChild;
		igObject* _nextSibling;
		auint16_t _index;
		auint16_t _sizeofSize;
		auint16_t _properties;
		auint16_t _requiredAlignment;
		igTVector<void*> _metaFunctions;
		igObjectList* _attributes;
#if (TARGET_GAME >= SKYTT_BEGIN) && (TARGET_GAME <= SKYTT_END)
		aint16_t _id;
		aint16_t _tfbID;
#else
		aint32_t _id;
#endif // (TARGET_GAME >= SKYTT_BEGIN) && (TARGET_GAME <= SKYTT_END)
#else // TARGET_GAME >= SKYSA_WIIU_BEGIN
		aint32_t _id;
private:
		const char* _name;
public:
		igTVector<igMetaField*> _metaFields;
		aint32_t _instanceCount;
		aint32_t _sizeofSize;
		void** _vTablePointer;
		igMetaObject* _parent;
		igObject* _lastChild;
		igObject* _nextSibling;
		aint16_t _index;
		auint16_t _properties;
		auint16_t _requiredAlignment;
		igTVector<void*> _metaFunctions;
		igObjectList* _attributes;
#endif // TARGET_GAME >= SKYSA_WIIU_BEGIN

		inline bool isOfType(const igMetaObject* other) const
		{
			return Core::igMetaObject_isOfType(this, other);
		}

		inline const char* getName()
		{
			asize_t name = reinterpret_cast<asize_t>(_name);
#if TARGET_WII // They use the most significant bit to store something on wii
			name = name | 0x80000000;
#endif // TARGET_WII
			return reinterpret_cast<const char*>(name);
		}
	};
}

namespace DotNet
{
	class igDotNetMetaObject : public Core::igMetaObject
	{
	public:
		Core::igObject* _methods;
		Core::igObject* _interfaces;
		Core::igObject* _vTable;
		Core::igMetaObject* _boundMeta;
		Core::igObject* _templateParameters;
		Core::igMetaObject* _patchMeta;
#if TARGET_GAME >= SKYIM_01_00_00 // These only exist on imaginators up (issue #3)
		void* _dotNetFieldNames;
		void* _cppFieldNames;
		aint32_t _exposedFieldCount;
		void* _cppMethods;
		void* _cppMethodNames;
		aint32_t _cppMethodCount;
#endif // TARGET_GAME >= SKYIM_01_00_00
		bool _isInterface;
		bool _baseMethodsInherited;
	};
}