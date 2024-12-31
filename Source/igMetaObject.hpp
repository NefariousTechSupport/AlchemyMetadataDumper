#pragma once

#include "igBaseMeta.hpp"
#include "igObjectList.hpp"
#include "igVector.hpp"

namespace Core
{
	class igMetaField;

	class igMetaObject : public igBaseMeta
	{
	public:
		igTVector<igMetaField*> _metaFields;
		int32_t _instanceCount;
		void** _vTablePointer;
		igMetaObject* _parent;
		igObject* _lastChild;
		igObject* _nextSibling;
		uint16_t _index;
		uint16_t _sizeofSize;
		uint16_t _properties;
		uint16_t _requiredAlignment;
		igTVector<void*> _metaFunctions;
		igObjectList* _attributes;
		int32_t _id;

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
		int32_t _exposedFieldCount;
		void* _cppMethods;
		void* _cppMethodNames;
		int32_t _cppMethodCount;
#endif // TARGET_GAME >= SKYIM_01_00_00
		bool _isInterface;
		bool _baseMethodsInherited;
	};
}