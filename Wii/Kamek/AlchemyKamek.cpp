#include "AlchemyKamek.h"



void OSFatal_placeholder(u32* fg, u32* bg, const char* str, ...)
{
	// Should be enough
	*(const char**)0 = str;
}
