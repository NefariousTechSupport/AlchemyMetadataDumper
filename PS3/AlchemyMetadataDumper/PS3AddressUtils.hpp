#pragma once

#include "alchemystdint.h"

#ifndef PS3_TOC_BASE
#error "You must define PS3_TOC_BASE before including PS3AddressUtils.hpp, please do so on the line before"
#endif


struct opd_s
{
	auint32_t sub;
	auint32_t toc;
};


// Defines a PS3 method with its function address and toc
#define DefineMethodWithToc(address, toc, ret, name, ...) \
static const opd_s _##address##toc = { address, toc }; \
ret(*name)(__VA_ARGS__) = (ret(*)(__VA_ARGS__))&_##address##toc

// Defines a PS3 method with its function address
#define DefineMethod(address, ret, name, ...) DefineMethodWithToc(address, PS3_TOC_BASE, ret, name, __VA_ARGS__)