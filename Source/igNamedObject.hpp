#pragma once

#include "igObject.hpp"

namespace Core
{
	class igNamedObject : public igObject
	{
	public:
		const char* _name;
	};
}