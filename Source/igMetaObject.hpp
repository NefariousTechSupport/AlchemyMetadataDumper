#pragma once

#include "igBaseMeta.hpp"
#include "igObjectList.hpp"
#include "igVector.hpp"

namespace Core
{
	class igMetaField;

	class igMetaObject
#if TARGET_GAME >= SKYTT_BEGIN
	: public igBaseMeta
#else
	: public igObject
#endif // TARGET_GAME >= SKYTT_BEGIN
	{
	public:
#if TARGET_GAME >= SKYTT_BEGIN
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
#if TARGET_GAME <= SKYTT_END
		aint16_t _id;
		aint16_t _tfbID;
#else
		aint32_t _id;
#endif // TARGET_GAME >= SKYTT_01_00_00 && TARGET_GAME <= SKYTT_01_01_00
#else // TARGET_GAME >= SKYTT_BEGIN
		aint32_t _id;
		const char* _name;
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
#endif // TARGET_GAME >= SKYTT_BEGIN

		inline bool isOfType(const igMetaObject* other) const
		{
			return Core::igMetaObject_isOfType(this, other);
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