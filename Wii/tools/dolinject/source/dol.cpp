//=============================================================================
// Includes
//-----------------------------------------------------------------------------
#include "dol.h"

#include <byteswap.h>





//=============================================================================
// ByteSwapDolHeader : Byte swaps a dol header
//-----------------------------------------------------------------------------
void ByteSwapDolHeader(DolHeader* header)
{
	uint32_t* asInts = reinterpret_cast<uint32_t*>(header);
	for (uint8_t i = 0; i < sizeof(DolHeader) / sizeof(uint32_t); i++)
	{
		asInts[i] = bswap_32(asInts[i]);
	}
}
