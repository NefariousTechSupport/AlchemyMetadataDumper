#pragma once

#include "igName.hpp"

namespace Core
{
	class igObject;

	struct igHandle
	{
		struct Data
		{
			void* unknown;
			igName _name;
			igName _namespace;
			igObject* _object;
		};

		Data* _data;
	};
}