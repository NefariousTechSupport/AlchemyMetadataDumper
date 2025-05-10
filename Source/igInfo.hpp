#pragma once

#include "igNamedObject.hpp"

namespace Core
{
	class igInfo : public igNamedObject
	{
		igObject* _directory;
		bool _resolveState;
	};
}