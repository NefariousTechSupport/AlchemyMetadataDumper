#pragma once

#if __cplusplus < 201103L
#define nullptr (reinterpret_cast<void*>(0))

#include "cpp11hacks_static_assert.hpp"
#endif // __cplusplus < 201103L
