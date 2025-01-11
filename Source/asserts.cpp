#include <assert.h>

#include "igMetaField.hpp"

// Runtime assert because we don't have static_assert on C++03

void assert_igMetaField()
{
	assert(sizeof(Core::fieldProperties) == 4);
}