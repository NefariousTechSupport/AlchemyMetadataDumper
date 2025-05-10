#pragma once

#include "tfbScriptObject.hpp"
#include "igNamedObject.hpp"

namespace tfbScript
{
	class tfbScriptObject : public Core::igNamedObject
	{
	public:
		void* _getFunc;
		void* _setFunc;
		void* _resetFunc;
	};
}