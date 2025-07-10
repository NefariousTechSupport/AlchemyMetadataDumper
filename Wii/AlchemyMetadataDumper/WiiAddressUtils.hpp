#pragma once

#include "alchemystdint.h"

// Defines a Wii method with its function address
#define DefineMethod(address, ret, name, ...) \
ret(*name)(__VA_ARGS__) = (ret(*)(__VA_ARGS__))address;
