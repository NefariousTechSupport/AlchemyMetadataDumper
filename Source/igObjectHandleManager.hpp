#pragma once

#include "igDataList.hpp"
#include "igObject.hpp"
#include "igName.hpp"

namespace Core
{
	struct igHandle;

	// Technically inherits from igSingleton
	class igObjectHandleManager : public igObject
	{
	public:
		igTDataList<const char*>* _systemNamespaces;
		void* _resolveToHandleFunction;
		bool _assertHandleOverwrites;
		aint32_t _highWaterCount;
		igName _runtimeHandleName;
		auint32_t _runtimeHandleId;
		igObject* _handlePool;
		igTUHashTable<igHandle, auint64_t>* _handleTable;
		igObject* _objectToHandleTable;
		igObject* _handleRedirectPool;
		igObject* _lock;
		igObject* _handleLock;
		// There's more here
	};
}