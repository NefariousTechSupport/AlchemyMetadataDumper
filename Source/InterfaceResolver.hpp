#pragma once

#include "igInfo.hpp"
#include "igObjectList.hpp"

namespace tfbScript
{
	class tfbScriptObject;

	class InterfaceResolver : public Core::igInfo
	{
	public:
		Core::igTObjectList<tfbScriptObject>* _lists[2];
	};
}